#ifndef HELPERS_H
#define HELPERS_H
#include "../src/cub.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// NOTE: Ignores alpha channel
int write_ppm(Cubc_Canvas canvas, const char* out) {
    FILE* file = fopen(out, "wb");

    char fmt_buf[512] = {0};

    if (file == NULL) {
        return 1;
    }

    fputs("P3\n", file);
    snprintf(fmt_buf, 512, "%zu %zu\n", canvas.w, canvas.h);
    fputs(fmt_buf, file);
    fputs("255\n", file);
    memset(fmt_buf, 0, 512);

    for (int i = 0; i < canvas.w * canvas.h; i++) {
        const uint32_t color = canvas.pixels[i];
        uint8_t r            = (color >> (3 * 8)) & 0xff;
        uint8_t g            = (color >> (2 * 8)) & 0xff;
        uint8_t b            = (color >> (1 * 8)) & 0xff;
        snprintf(fmt_buf, 512, "%d %d %d\n", r, g, b);
        fputs(fmt_buf, file);
        memset(fmt_buf, 0, 512);
    }

    fclose(file);

    return 0;
}
char* read_file(const char* name) {
    FILE* file = fopen(name, "rb");
    if (file == NULL) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t len = ftell(file) + 1;
    fseek(file, 0, SEEK_SET);

    char* str = (char*) malloc(sizeof(char) * len);
    fread(str, len, 1, file);
    str[len] = 0;

    return str;
}

#endif // !HELPERS_H
