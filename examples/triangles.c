#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CUBC_IMPLEMENTATION
#include "../src/cub.c"
#include "helpers.h"

int main() {
    Cubc_Canvas canvas = {
        .w      = 600,
        .h      = 600,
        .pixels = malloc(sizeof(uint32_t) * 600 * 600),
    };
    memset(canvas.pixels, 0, 600 * 600);
    Cubc_CanvasWireframeTriangle(&canvas, 100, 100, 200, 200, 100, 200,
                                 CC_WHITE);
    write_ppm(canvas, "testing_out.ppm");
}
