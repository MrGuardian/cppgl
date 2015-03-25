#pragma once
#include "Color.h"

namespace CppGL {

    class Bitmap {
    public:
        Bitmap(int width, int height, const Color *clearColor = nullptr) : _width(width), _height(height), _pixelsSet(0) {
            _pixels = vector<Color>(_width * _height, Color::BLACK);
            Color pixelColor;
            if (clearColor == nullptr) {
                pixelColor = Color::BLACK;
            }
            else {
                pixelColor = *clearColor;
            }
            clear(pixelColor);
        }

        void setPixelColor(int x, int y, const Color &pixel) {
            _pixelsSet += 1;
            int pixelPosition = calculatePixelPosition(x, y);
            if (pixelPosition != -1) {
                _pixels[pixelPosition] = pixel;
            }
        }

        const Color& getPixelColor(int x, int y) const {
            int pixelPosition = calculatePixelPosition(x, y);
            return pixelPosition != -1 ? _pixels[pixelPosition] : Color::WHITE;
        }

        void clear(const Color &clearColor) {
            for (auto& pixel : _pixels) {
                pixel = clearColor;
            }
        }

        const int width() const { return _width; }
        const int height() const { return _height; }

        const int pixelsSet() const { return _pixelsSet; }

        const vector<Color>& getPixels() const {
            return _pixels;
        }

    private:
        int _width;
        int _height;
        vector<Color> _pixels;
        int _pixelsSet;

        /*
        returns valid pixel position within contiguous array of pixels or -1 if not within the bounds
        */
        const int calculatePixelPosition(int x, int y) const {
            int pixelPosition = (_height - 1 - y) * _width + x;
            return (pixelPosition <= _pixels.size() - 1) ? pixelPosition : -1;
        }
    };
}