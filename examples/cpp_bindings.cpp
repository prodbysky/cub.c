#define CUBC_IMPLEMENTATION
#include "../bindings/cpp/cub.cpp"
#include "helpers.h"

int main() {
    Cubc::Canvas canvas(1280, 720, Cubc::Red);
    Cubc::Canvas pog("pog.png");
    canvas.BlitCanvas(pog, 100, 100, 0.5, 0.5);
    write_ppm(canvas.CRepr(), "testing_out.ppm");
}
