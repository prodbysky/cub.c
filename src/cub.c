#ifndef CUBC
#define CUBC

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t* pixels;
    size_t w, h;
} Cubc_Canvas;

typedef union {
    uint32_t color;
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
} Cubc_Color;

#define CC_BLACK                                                               \
    (Cubc_Color) { .color = 0x000000ff }

#define CC_RED                                                                 \
    (Cubc_Color) { .color = 0xff0000ff }

#define CC_GREEN                                                               \
    (Cubc_Color) { .color = 0x00ff00ff }

#define CC_BLUE                                                                \
    (Cubc_Color) { .color = 0x0000ffff }

#define CC_WHITE                                                               \
    (Cubc_Color) { .color = 0xffffffff }

typedef struct {
    int32_t x, y;
} Cubc_V2i;

typedef struct {
    uint32_t x, y;
} Cubc_V2u;

typedef struct {
    float x, y;
} Cubc_V2f;

typedef struct {
    float x, y, w, h;
} Cubc_Rect;

Cubc_Canvas Cubc_CanvasFromImage(const char* file_name);

void Cubc_CanvasBlitCanvas(Cubc_Canvas* dest, const Cubc_Canvas* src,
                           uint32_t x, uint32_t y, float scale_x,
                           float scale_y);

void Cubc_CanvasBlitCanvasV(Cubc_Canvas* dest, const Cubc_Canvas* src,
                            Cubc_V2u pos, Cubc_V2f scale);

void Cubc_CanvasBlitCanvasR(Cubc_Canvas* dest, const Cubc_Canvas* src,
                            Cubc_Rect rect);

void Cubc_CanvasClear(Cubc_Canvas* canvas, Cubc_Color color);

void Cubc_CanvasPixel(Cubc_Canvas* canvas, uint32_t x, uint32_t y,
                      Cubc_Color color);
void Cubc_CanvasPixelV(Cubc_Canvas* canvas, Cubc_V2u pos, Cubc_Color color);

void Cubc_CanvasLine(Cubc_Canvas* canvas, uint32_t x0, uint32_t y0, uint32_t x1,
                     uint32_t y1, Cubc_Color color);
void Cubc_CanvasLineV(Cubc_Canvas* canvas, Cubc_V2u begin, Cubc_V2u end,
                      Cubc_Color color);

void Cubc_CanvasWireframeTriangle(Cubc_Canvas* canvas, uint32_t x0, uint32_t y0,
                                  uint32_t x1, uint32_t y1, uint32_t x2,
                                  uint32_t y2, Cubc_Color color);

void Cubc_CanvasWireframeTriangleV(Cubc_Canvas* canvas, Cubc_V2u pos0,
                                   Cubc_V2u pos1, Cubc_V2u pos2,
                                   Cubc_Color color);

void Cubc_CanvasTriangle(Cubc_Canvas* canvas, uint32_t x0, uint32_t y0,
                         uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                         Cubc_Color color);

void Cubc_CanvasTriangleV(Cubc_Canvas* canvas, Cubc_V2u pos0, Cubc_V2u pos1,
                          Cubc_V2u pos2, Cubc_Color color);

void Cubc_CanvasRect(Cubc_Canvas* canvas, uint32_t x, uint32_t y, uint32_t w,
                     uint32_t h, Cubc_Color color);
void Cubc_CanvasRectV(Cubc_Canvas* canvas, Cubc_V2f pos, Cubc_V2f size,
                      Cubc_Color color);
void Cubc_CanvasRectR(Cubc_Canvas* canvas, Cubc_Rect rect, Cubc_Color color);

#define CUBC_CANVAS_AT(canvas, x, y) (canvas).pixels[(x) + (y) * (canvas).w]

#define SWAP(x, y)                                                             \
    {                                                                          \
        typeof(x) temp = x;                                                    \
        x              = y;                                                    \
        y              = temp;                                                 \
    }

#ifdef CUBC_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Cubc_Canvas Cubc_CanvasFromImage(const char* file_name) {
    int x, y, comp;
    uint8_t* img_data  = stbi_load(file_name, &x, &y, &comp, 4);
    Cubc_Canvas canvas = {
        .w      = x,
        .h      = y,
        .pixels = malloc(sizeof(uint32_t) * x * y),
    };
    int m = 0;
    for (int i = 0; i < x * y * 4; i += 4) {
        uint8_t r        = img_data[i + 3];
        uint8_t g        = img_data[i + 2];
        uint8_t b        = img_data[i + 1];
        uint8_t a        = img_data[i];
        canvas.pixels[m] = r | (g << 8) | (b << 16) | (a << 24);
        m++;
    }
    return canvas;
}

void Cubc_CanvasBlitCanvas(Cubc_Canvas* dest, const Cubc_Canvas* src,
                           uint32_t x, uint32_t y, float scale_x,
                           float scale_y) {
    for (uint32_t dest_y = 0; dest_y < (uint32_t) (src->h * scale_y);
         dest_y++) {
        for (uint32_t dest_x = 0; dest_x < (uint32_t) (src->w * scale_x);
             dest_x++) {
            uint32_t src_x = (uint32_t) (dest_x / scale_x);
            uint32_t src_y = (uint32_t) (dest_y / scale_y);

            if (src_x >= src->w || src_y >= src->h) {
                continue;
            }

            uint32_t dest_pos_x = x + dest_x;
            uint32_t dest_pos_y = y + dest_y;

            if (dest_pos_x >= dest->w || dest_pos_y >= dest->h) {
                continue;
            }

            Cubc_CanvasPixel(dest, dest_pos_x, dest_pos_y,
                             (Cubc_Color) CUBC_CANVAS_AT(*src, src_x, src_y));
        }
    }
}

void Cubc_CanvasBlitCanvasV(Cubc_Canvas* dest, const Cubc_Canvas* src,
                            Cubc_V2u pos, Cubc_V2f scale) {
    Cubc_CanvasBlitCanvas(dest, src, pos.x, pos.y, scale.x, scale.y);
}

void Cubc_CanvasBlitCanvasR(Cubc_Canvas* dest, const Cubc_Canvas* src,
                            Cubc_Rect rect) {
    Cubc_CanvasBlitCanvas(dest, src, rect.x, rect.y, rect.w, rect.h);
}

void Cubc_CanvasClear(Cubc_Canvas* canvas, Cubc_Color color) {
    for (int i = 0; i < canvas->w * canvas->h; i++) {
        canvas->pixels[i] = color.color;
    }
}
void Cubc_CanvasPixel(Cubc_Canvas* canvas, uint32_t x, uint32_t y,
                      Cubc_Color color) {
    if (x + y * canvas->w < canvas->w * canvas->h) {
        CUBC_CANVAS_AT(*canvas, x, y) = color.color;
    }
}

void Cubc_CanvasPixelV(Cubc_Canvas* canvas, Cubc_V2u pos, Cubc_Color color) {
    CUBC_CANVAS_AT(*canvas, pos.x, pos.y) = color.color;
}

void Cubc_CanvasLine(Cubc_Canvas* canvas, uint32_t x0, uint32_t y0, uint32_t x1,
                     uint32_t y1, Cubc_Color color) {
    bool steep = false;
    if (x0 - x1 < y0 - y1) {
        SWAP(x0, y0);
        SWAP(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        SWAP(x0, x1);
        SWAP(y0, y1);
    }
    int dx      = x1 - x0;
    int dy      = y1 - y0;
    int derror2 = dy * 2;
    int error2  = 0;
    int y       = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            Cubc_CanvasPixel(canvas, y, x, color);
        } else {
            Cubc_CanvasPixel(canvas, x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y      += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void Cubc_CanvasLineV(Cubc_Canvas* canvas, Cubc_V2u begin, Cubc_V2u end,
                      Cubc_Color color) {
    Cubc_CanvasLine(canvas, begin.x, begin.y, end.x, end.y, color);
}

void Cubc_CanvasWireframeTriangle(Cubc_Canvas* canvas, uint32_t x0, uint32_t y0,
                                  uint32_t x1, uint32_t y1, uint32_t x2,
                                  uint32_t y2, Cubc_Color color) {
    Cubc_CanvasLine(canvas, x0, y0, x1, y1, color);
    Cubc_CanvasLine(canvas, x1, y1, x2, y2, color);
    Cubc_CanvasLine(canvas, x2, y2, x0, y0, color);
}

void Cubc_CanvasWireframeTriangleV(Cubc_Canvas* canvas, Cubc_V2u pos0,
                                   Cubc_V2u pos1, Cubc_V2u pos2,
                                   Cubc_Color color) {
    Cubc_CanvasLineV(canvas, pos0, pos1, color);
    Cubc_CanvasLineV(canvas, pos1, pos2, color);
    Cubc_CanvasLineV(canvas, pos2, pos0, color);
}

int _Cubc_InterpolateX(uint32_t y0, uint32_t x0, uint32_t y1, uint32_t x1,
                       uint32_t y) {
    if (y1 == y0)
        return x0;
    int64_t dx = (int64_t) (x1 - x0);
    int64_t dy = (int64_t) (y1 - y0);
    return x0 + (y - y0) * dx / dy;
}

int _Cubc_InterpolateXV(Cubc_V2u pos0, Cubc_V2u pos1, uint32_t y) {
    return _Cubc_InterpolateX(pos0.x, pos0.y, pos1.x, pos1.y, y);
}

void Cubc_CanvasTriangle(Cubc_Canvas* canvas, uint32_t x0, uint32_t y0,
                         uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2,
                         Cubc_Color color) {
    if (y1 < y0) {
        SWAP(y0, y1);
        SWAP(x0, x1);
    }
    if (y2 < y0) {
        SWAP(y2, y0);
        SWAP(x2, x0);
    }
    if (y2 < y1) {
        SWAP(y2, y1);
        SWAP(x2, x1);
    }

    for (int y = y0; y <= y1; y++) {
        uint32_t x_begin = _Cubc_InterpolateX(y0, x0, y2, x2, y);
        uint32_t x_end   = _Cubc_InterpolateX(y0, x0, y1, x1, y);
        Cubc_CanvasLine(canvas, x_begin, y, x_end, y, color);
    }

    for (int y = y1; y <= y2; y++) {
        uint32_t x_begin = _Cubc_InterpolateX(y0, x0, y2, x2, y);
        uint32_t x_end   = _Cubc_InterpolateX(y1, x1, y2, x2, y);
        Cubc_CanvasLine(canvas, x_begin, y, x_end, y, color);
    }
}

void Cubc_CanvasTriangleV(Cubc_Canvas* canvas, Cubc_V2u pos0, Cubc_V2u pos1,
                          Cubc_V2u pos2, Cubc_Color color) {
    Cubc_CanvasTriangle(canvas, pos0.x, pos0.y, pos1.x, pos1.y, pos2.x, pos2.y,
                        color);
}

void Cubc_CanvasRect(Cubc_Canvas* canvas, uint32_t x, uint32_t y, uint32_t w,
                     uint32_t h, Cubc_Color color) {
    for (int dy = y; dy <= y + h; dy++) {
        Cubc_CanvasLine(canvas, x, dy, x + w, dy, color);
    }
}
void Cubc_CanvasRectV(Cubc_Canvas* canvas, Cubc_V2f pos, Cubc_V2f size,
                      Cubc_Color color) {

    Cubc_CanvasRect(canvas, pos.x, pos.y, size.x, size.y, color);
}
void Cubc_CanvasRectR(Cubc_Canvas* canvas, Cubc_Rect rect, Cubc_Color color) {
    Cubc_CanvasRect(canvas, rect.x, rect.y, rect.w, rect.h, color);
}

#endif

#endif
