#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CUBC_IMPLEMENTATION
#include "../src/cub.c"
#include "helpers.h"

int main() {
    Cubc_Canvas pog    = Cubc_CanvasFromImage("pog.png");
    Cubc_Canvas canvas = {
        .w      = pog.w,
        .h      = pog.h,
        .pixels = malloc(sizeof(uint32_t) * pog.w * pog.h),
    };
    memset(canvas.pixels, 0, canvas.h * canvas.w);
    Cubc_CanvasBlitCanvas(&canvas, &pog, 100, 100, 0.5, 0.5);
    write_ppm(canvas, "testing_out.ppm");
}
