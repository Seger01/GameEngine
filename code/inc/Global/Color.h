#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

class Color {
public:
    Color() : r(0), g(0), b(0), a(255) {}
    Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}
    Color(int r, int g, int b) : r(r), g(g), b(b), a(255) {}
    ~Color() {}

    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

#endif // COLOR_H
