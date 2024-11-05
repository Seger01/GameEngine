#pragma once

struct Point
{
    float x;
    float y;

    // Default constructor
    Point() : x(0), y(0) {}  // Initializes x and y to 0 by default

    // Constructor with parameters
    Point(float x, float y) : x(x), y(y) {}
};
