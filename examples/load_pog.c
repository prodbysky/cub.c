#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CUBC_IMPLEMENTATION
#include "../src/cub.c"
#include "helpers.h"

int main() {
    Cubc_Canvas canvas = Cubc_CanvasFromImage("pog.png");
    // memset(canvas.pixels, 0, 600 * 600);
    write_ppm(canvas, "testing_out.ppm");
}
