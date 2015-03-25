#pragma once
#include "Utils.h"

namespace CppGL {
    class ZBuffer {
    public:
        ZBuffer(const int width, const int height) : _width(width), _height(height) {
            _pixels = vector<double>(_width * _height, 0.0);
        }

        const double getPixelDepth(const int x, const int y) const {
            int pixelPosition = calculatePixelPosition(x, y);
            return pixelPosition != -1 ? _pixels[pixelPosition] : 0.0;
        }

        const bool setPixelDepth(const int x, const int y, const double z) {
            if (getPixelDepth(x, y) > z) {
                int pixelPosition = calculatePixelPosition(x, y);
                _pixels[pixelPosition] = pixelPosition != -1 ? z : 0.0;
                return true;
            }
            else {
                return false;
            }
        }

        const double interpolate(const double z, const double min, const double max) {
            return (z - min) / (max - min);
        }

    private:
        vector<double> _pixels;
        int _width;
        int _height;

        const int calculatePixelPosition(const int x, const int y) const {
            int pixelPosition = (_height - 1 - y) * _width + x;
            return (pixelPosition <= _pixels.size() - 1) ? pixelPosition : -1;
        }
    };
}