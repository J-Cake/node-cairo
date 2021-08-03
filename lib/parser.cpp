#include <napi.h>
#include <vector>
#include <iostream>

#include "lib.h"

bool getColour(Napi::Value value, Colour* colour) {
    if (!value.IsArray())
        return false;

    Napi::Array colour_tuple = value.As<Napi::Array>();

    if (colour_tuple.Length() != 3)
        return false;

    colour->r = colour_tuple.Get("0").As<Napi::Number>().FloatValue();
    colour->g = colour_tuple.Get("1").As<Napi::Number>().FloatValue();
    colour->b = colour_tuple.Get("2").As<Napi::Number>().FloatValue();

    return true;
}

bool getRect(Napi::Value value, Rect* rect) {
    if (!value.IsObject())
        return false;

    Napi::Object rect_obj = value.As<Napi::Object>();

    if (!rect_obj.Has("x") || !rect_obj.Has("y") || !rect_obj.Has("width") || !rect_obj.Has("height"))
        return false;

    rect->x = rect_obj.Get("x").As<Napi::Number>().FloatValue();
    rect->y = rect_obj.Get("y").As<Napi::Number>().FloatValue();
    rect->width = rect_obj.Get("width").As<Napi::Number>().FloatValue();
    rect->height = rect_obj.Get("height").As<Napi::Number>().FloatValue();

    if (rect_obj.Has("radius"))
        rect->radius = rect_obj.Get("radius").As<Napi::Number>().DoubleValue();

    return true;
}

bool getTextProps(Napi::Value value, TextProps* textProps) {
    if (!value.IsObject())
        return false;

    Napi::Object textProps_obj = value.As<Napi::Object>();

    if (textProps_obj.Has("font"))
        textProps->font = textProps_obj.Get("font").As<Napi::String>().Utf8Value();

    if (textProps_obj.Has("size"))
        textProps->size = textProps_obj.Get("size").As<Napi::Number>().FloatValue();

    if (textProps_obj.Has("style"))
        textProps->style = (TextStyle) textProps_obj.Get("style").As<Napi::Number>().Uint32Value();

    if (textProps_obj.Has("align"))
        textProps->align = (TextAlign) textProps_obj.Get("align").As<Napi::Number>().Uint32Value();

    return true;
}

bool getDevice(Napi::Value value, Device* device) {
    if (!value.IsObject())
        return false;

    Napi::Object device_obj = value.As<Napi::Object>();

    if (device_obj.Has("width"))
        device->width = device_obj.Get("width").As<Napi::Number>().FloatValue();

    if (device_obj.Has("height"))
        device->height = device_obj.Get("height").As<Napi::Number>().FloatValue();

    if (device_obj.Has("device"))
        device->device = device_obj.Get("device").As<Napi::String>().Utf8Value();

    if (device_obj.Has("data"))
        device->buffer = device_obj.Get("data").As<Napi::Buffer<unsigned char>>();

    device->data = device->buffer.Data();

    return true;
}
