#pragma once
#include "Utils.h"
#include <iostream>

namespace CppGL {

    class Color {
    public:
        uchar r;
        uchar g;
        uchar b;
        uchar a;

        Color() : r(0), g(0), b(0), a(255) {}

        Color(uchar red, uchar green, uchar blue, uchar alpha) : r(red), g(green), b(blue), a(alpha) {}

        static const Color GenerateRandom() {
            return Color(rand() % 255, rand() % 255, rand() % 255, 255);
        }

        static const Color RED;
        static const Color BLACK;
        static const Color WHITE;
        static const Color GREEN;
    };

    const Color Color::RED = Color(255, 0, 0, 255);
    const Color Color::BLACK = Color(0, 0, 0, 255);
    const Color Color::WHITE = Color(255, 255, 255, 255);
    const Color Color::GREEN = Color(0, 255, 0, 255);
}