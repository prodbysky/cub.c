#ifndef CUBCPP
#define CUBCPP

#include "../../src/cub.c"

#include <cstdint>
#include <cstdlib>
namespace Cubc {

    union Color {
        uint32_t color;
        struct {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };

    public:
        Cubc_Color CRepr() const { return {color}; }
        Cubc_Color Multiply(const Color& other) const;
        Cubc_Color Screen(const Color& other) const;
        Cubc_Color Overlay(const Color& other) const;
        Cubc_Color HardLight(const Color& other) const;
        Cubc_Color SoftLight(const Color& other) const;
    };

    constexpr Color Black = {.color = 0x000000ff};
    constexpr Color Red   = {.color = 0xff0000ff};
    constexpr Color Green = {.color = 0x00ff00ff};
    constexpr Color Blue  = {.color = 0x0000ffff};
    constexpr Color White = {.color = 0xffffffff};

    struct V2i {
        int32_t x, y;
    };

    struct V2u {
        uint32_t x, y;
    };

    struct V2f {
        float x, y;
    };

    struct Rect {
        float x, y, w, h;
    };

    class Canvas {
    public:
        Canvas(size_t w = 1280, size_t h = 720, Color c = Black);
        Canvas(const char* file_name);
        ~Canvas();

        void BlitCanvas(const Canvas& src, uint32_t x = 0, uint32_t y = 0,
                        float scale_x = 1.0, float scale_y = 1.0);

        void BlitCanvas(const Canvas& src, V2u pos = {0, 0},
                        V2f scale = {1, 1});

        void BlitCanvas(const Canvas& src, Rect rect = {0, 0, 1, 1});

        void Clear(Color c = Black);

        void Pixel(uint32_t x, uint32_t y, Color c);
        void Pixel(V2u pos, Color c);

        void Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
                  Color color);
        void Line(V2u begin, V2u end, Color color);

        void WireframeTriangle(uint32_t x0, uint32_t y0, uint32_t x1,
                               uint32_t y1, uint32_t x2, uint32_t y2,
                               Color color);
        void WireframeTriangle(V2u pos0, V2u pos1, V2u pos2, Color color);

        void Triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
                      uint32_t x2, uint32_t y2, Color color);
        void Triangle(V2u pos0, V2u pos1, V2u pos2, Color color);

        void Rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, Color color);
        void Rect(V2f pos, V2f size, Color color);
        void Rect(struct Rect rect, Color color);
        Cubc_Canvas CRepr() const {
            return {
                .pixels = pixels,
                .w      = w,
                .h      = h,
            };
        }

    private:
        size_t w, h;
        uint32_t* pixels;
    };
} // namespace Cubc

#ifdef CUBC_IMPLEMENTATION
namespace Cubc {
#include "../../src/cub.c"

    Cubc_Color Color::Multiply(const Color& other) const {
        return Cubc_ColorMultiplyBlend(CRepr(), other.CRepr());
    }
    Cubc_Color Color::Screen(const Color& other) const {
        return Cubc_ColorScreenBlend(CRepr(), other.CRepr());
    }
    Cubc_Color Color::Overlay(const Color& other) const {
        return Cubc_ColorOverlayBlend(CRepr(), other.CRepr());
    }
    Cubc_Color Color::HardLight(const Color& other) const {
        return Cubc_ColorHardLightBlend(CRepr(), other.CRepr());
    }
    Cubc_Color Color::SoftLight(const Color& other) const {
        return Cubc_ColorSoftLightBlend(CRepr(), other.CRepr());
    }

    Canvas::Canvas(size_t w, size_t h, Color c) : w(w), h(h) {
        pixels = new uint32_t[w * h];
        for (int i = 0; i < w * h; i++) {
            pixels[i] = c.color;
        }
    }
    Canvas::Canvas(const char* file_name) {
        Cubc_Canvas canvas = Cubc_CanvasFromImage(file_name);
        w                  = canvas.w;
        h                  = canvas.h;
        pixels             = canvas.pixels;
    }
    Canvas::~Canvas() { free(pixels); }
    void Canvas::Clear(Color c) {
        for (int i = 0; i < w * h; i++) {
            pixels[i] = c.color;
        }
    }

    void Canvas::Pixel(uint32_t x, uint32_t y, Color c) {
        auto repr = CRepr();
        Cubc_CanvasPixel(&repr, x, y, c.CRepr());
    }
    void Canvas::Pixel(V2u pos, Color c) { Pixel(pos.x, pos.y, c); }

    void Canvas::BlitCanvas(const Canvas& src, uint32_t x, uint32_t y,
                            float scale_x, float scale_y) {
        auto repr_dest = CRepr();
        auto repr_src  = src.CRepr();
        Cubc_CanvasBlitCanvas(&repr_dest, &repr_src, x, y, scale_x, scale_x);
    }

    void Canvas::BlitCanvas(const Canvas& src, V2u pos, V2f scale) {
        BlitCanvas(src, pos.x, pos.y, scale.x, scale.y);
    }

    void Canvas::BlitCanvas(const Canvas& src, struct Rect rect) {
        BlitCanvas(src, rect.x, rect.y, rect.w, rect.h);
    }

    void Canvas::Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
                      Color color) {
        auto repr = CRepr();
        Cubc_CanvasLine(&repr, x0, y0, x1, y1, color.CRepr());
    }

    void Canvas::Line(V2u begin, V2u end, Color color) {
        Line(begin.x, begin.y, end.x, end.y, color);
    }

    void Canvas::WireframeTriangle(uint32_t x0, uint32_t y0, uint32_t x1,
                                   uint32_t y1, uint32_t x2, uint32_t y2,
                                   Color color) {
        auto repr = CRepr();
        Cubc_CanvasWireframeTriangle(&repr, x0, y0, x1, y1, x2, y2,
                                     color.CRepr());
    }
    void Canvas::WireframeTriangle(V2u pos0, V2u pos1, V2u pos2, Color color) {
        WireframeTriangle(pos0.x, pos0.y, pos1.x, pos1.y, pos2.x, pos2.y,
                          color);
    }

    void Canvas::Triangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1,
                          uint32_t x2, uint32_t y2, Color color) {
        auto repr = CRepr();
        Cubc_CanvasTriangle(&repr, x0, y0, x1, y1, x2, y2, color.CRepr());
    }
    void Canvas::Triangle(V2u pos0, V2u pos1, V2u pos2, Color color) {
        Triangle(pos0.x, pos0.y, pos1.x, pos1.y, pos2.x, pos2.y, color);
    }

    void Canvas::Rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                      Color color) {
        auto repr = CRepr();
        Cubc_CanvasRect(&repr, x, y, w, h, color.CRepr());
    }
    void Canvas::Rect(V2f pos, V2f size, Color color) {
        Rect(pos.x, pos.y, size.x, size.y, color);
    }
    void Canvas::Rect(struct Rect rect, Color color) {
        Rect(rect.x, rect.y, rect.w, rect.h, color);
    }

} // namespace Cubc
#endif

#endif
