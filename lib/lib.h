#pragma once

#include <vector>

#include <napi.h>

#include <cairo.h>
#include <pango/pango.h>
#include <pango/pangocairo.h>

struct Colour {
    double r;
    double g;
    double b;
};

struct Rect {
    double x;
    double y;
    double width;
    double height;

    double radius;
};

struct Coordinate {
    double x;
    double y;
};

struct Size {
    double width;
    double height;
};

struct TextDimens {
    double size;
    double width;
    double height;
};

enum TextStyle {
    Bold = 0b0001,
    Italic = 0b0010,
    Underlined = 0b0100,
    Strikethrough = 0b1000,
};

enum TextAlign {
    Left = 0b001,
    Top = 0b010,
    Centre = 0b100,
    Right = 0b011,
    Bottom = 0b110,
    Justify = 0b111
};

struct TextProps {
    TextAlign align;
    TextStyle style;
    double size = 11;
    std::string font = "sans";
};

struct Device {
    double width;
    double height;

    cairo_t* ctx;
    cairo_surface_t* canvas;

    Device* parent = nullptr;

    PangoWeight bold = PANGO_WEIGHT_BOLD;
    PangoStyle italic = PANGO_STYLE_ITALIC;

    std::string device;
    Napi::Buffer<unsigned char> buffer;
    void* data;
};

Napi::Function fillFn(Napi::Env env, Device *dev);
Napi::Function rectFn(Napi::Env env, Device *dev);
Napi::Function textFn(Napi::Env env, Device *dev);
Napi::Function destroryFn(Napi::Env env, Device *ctx);
Napi::Function flushFn(Napi::Env env, Device *dev);
Napi::Function layerFn(Napi::Env env, Device *dev);
Napi::Object populateDevice(Napi::Env env, Device *device);
static Napi::Object create(const Napi::CallbackInfo &info);
