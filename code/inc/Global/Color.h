#pragma once

class Color {
public:
    Color() : r(0), g(0), b(0), a(255) {}
    Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}
    Color(int r, int g, int b) : r(r), g(g), b(b), a(255) {}
    ~Color() {}

    int r;
    int g;
    int b;
    int a;
};
