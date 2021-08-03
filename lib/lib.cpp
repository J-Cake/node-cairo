#include <napi.h>
#include <iostream>
#include <math.h>

#include <cairo.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

#include "./lib.h"
#include "./parser.h"

Napi::Function fillFn(Napi::Env env, Device* dev) {
    return Napi::Function::New(env, [=](const Napi::CallbackInfo &info) {
        Napi::Env env = info.Env();

        Colour colour{0, 0, 0};

        if (info.Length() != 1 || !getColour(info[0], &colour))
            Napi::TypeError::New(env, "Expected colour: rgb").ThrowAsJavaScriptException();

        cairo_set_source_rgb(dev->ctx, colour.r / 0xff.0p0f, colour.g / 0xff.0p0f, colour.b / 0xff.0p0f);
    });
}

Napi::Function rectFn(Napi::Env env, Device* dev) {
    return Napi::Function::New(env, [=](const Napi::CallbackInfo &info) {
        Napi::Env env = info.Env();

        Rect rect{0, 0, 0, 0};

        if (info.Length() != 1 || !getRect(info[0], &rect))
            Napi::TypeError::New(env, "Expected Rect: Rect").ThrowAsJavaScriptException();

        double radius = rect.radius;
        double degrees = M_PI / 180.0;

        cairo_new_sub_path (dev->ctx);
        cairo_arc (dev->ctx, rect.x + rect.width - radius, rect.y + radius, radius, -90 * degrees, 0 * degrees);
        cairo_arc (dev->ctx, rect.x + rect.width - radius, rect.y + rect.height - radius, radius, 0 * degrees, 90 * degrees);
        cairo_arc (dev->ctx, rect.x + radius, rect.y + rect.height - radius, radius, 90 * degrees, 180 * degrees);
        cairo_arc (dev->ctx, rect.x + radius, rect.y + radius, radius, 180 * degrees, 270 * degrees);
        cairo_close_path (dev->ctx);

        cairo_fill(dev->ctx);
    });
}

Napi::Function textFn(Napi::Env env, Device* dev) {
    return Napi::Function::New(env, [=](const Napi::CallbackInfo &info) -> Napi::Object {
        Napi::Env env = info.Env();
        Napi::Object textUtils = Napi::Object::New(env);

        TextProps props;
        if (!info[0].IsString() || !getTextProps(info[1], &props))
            Napi::TypeError::New(env, "Expected text: string, props: TextProps").ThrowAsJavaScriptException();

        PangoLayout *layout = pango_cairo_create_layout(dev->ctx);
        PangoFontDescription *desc = pango_font_description_new();

        TextStyle style = (TextStyle) props.style;

        pango_font_description_set_family(desc, props.font.c_str());
        pango_font_description_set_weight(desc, (style & TextStyle::Bold) ? dev->bold : PANGO_WEIGHT_NORMAL);
        pango_font_description_set_style(desc, (style & TextStyle::Italic) ? dev->italic : PANGO_STYLE_NORMAL);
        pango_font_description_set_size(desc, props.size * PANGO_SCALE);

        std::string str = info[0].As<Napi::String>().Utf8Value();
        pango_layout_set_text (layout, str.c_str(), str.length());

        pango_layout_set_alignment(layout, (props.align == TextAlign::Left) ? PANGO_ALIGN_LEFT : (props.align == TextAlign::Right) ? PANGO_ALIGN_RIGHT : PANGO_ALIGN_CENTER);

        pango_layout_set_font_description (layout, desc);

        textUtils.Set(Napi::String::New(env, "bounds"), Napi::Function::New(env, [=](const Napi::CallbackInfo &info) {
            Napi::Env env = info.Env();

            Rect bounds;
            if (!getRect(info[0], &bounds))
                Napi::TypeError::New(env, "Expected bounds: Rect").ThrowAsJavaScriptException();

            pango_layout_set_width(layout, bounds.width);
            pango_layout_set_height(layout, bounds.height);

            pango_cairo_update_layout(dev->ctx, layout);
        }));

        textUtils.Set(Napi::String::New(env, "paint"), Napi::Function::New(env, [=](const Napi::CallbackInfo &info) {
            Napi::Env env = info.Env();

            Rect bounds;
            if (!getRect(info[0], &bounds))
                Napi::TypeError::New(env, "Expected bounds: Rect").ThrowAsJavaScriptException();

            pango_layout_set_width(layout, bounds.width);
            pango_layout_set_height(layout, bounds.height);

            pango_cairo_update_layout(dev->ctx, layout);

            cairo_move_to(dev->ctx, bounds.x, bounds.y);
            pango_cairo_show_layout (dev->ctx, layout);
        }));

        PangoRectangle ink, logical;
        pango_layout_get_extents(layout, NULL, &logical);

        textUtils.Set(Napi::String::From(env, "width"), Napi::Number::From(env, 1. / PANGO_SCALE * logical.width));
        textUtils.Set(Napi::String::From(env, "height"), Napi::Number::From(env, 1. / PANGO_SCALE * logical.height));

        textUtils.Set(Napi::String::New(env, "baseline"), Napi::Number::From(env, pango_layout_get_baseline(layout) * 1./PANGO_SCALE));

        pango_font_description_free (desc);

        return textUtils;
    });
}

Napi::Function destroryFn(Napi::Env env, Device* ctx) {
    return Napi::Function::New(env, [=](const Napi::CallbackInfo &info) {
//         delete canvas;
    });
}

Napi::Function flushFn(Napi::Env env, Device* dev) {
    return Napi::Function::New(env, [=](const Napi::CallbackInfo &info) {
        cairo_surface_t* surface = cairo_get_target(dev->ctx);
        cairo_surface_flush(surface);

        auto source = cairo_image_surface_get_data(surface);
        memcpy(dev->data, source, dev->buffer.Length());
    });
}

static Napi::Object create(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    Device* device = new Device();

    if (!info[0].IsObject() || !getDevice(info[0], device))
        Napi::TypeError::New(env, "Expected device: Device").ThrowAsJavaScriptException();

    cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, device->width, device->height);
    cairo_t* ctx = cairo_create(surface);

    device->canvas = surface;
    device->ctx = ctx;

    Napi::Object interface = Napi::Object::New(env);

    interface.Set(Napi::String::New(env, "rect"), rectFn(env, device));
    interface.Set(Napi::String::New(env, "text"), textFn(env, device));

    interface.Set(Napi::String::New(env, "fill"), fillFn(env, device));

    interface.Set(Napi::String::New(env, "destrory"), destroryFn(env, device));
    interface.Set(Napi::String::New(env, "flush"), flushFn(env, device));

    return interface;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "create"), Napi::Function::New(env, create));

    return exports;
}

NODE_API_MODULE(cairo, Init)
