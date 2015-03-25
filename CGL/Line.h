#pragma once
#include "Vectors.h"
#include "LineData.h"

namespace CppGL {
    class Line {
    public:
		Line(Vec3d start, Vec3d end) : _data(start, end) {
            cacheCoeffs();
        }
		Line(double x0, double y0, double x1, double y1) : Line(Vec3d(x0, y0, 0), Vec3d(x1, y1, 0)) {}
        Line() : Line(0.0, 0.0, 0.0, 0.0) {};

        const double calculateY(const double x) const {
            if (_dx != 0) {
                return (x - start().x) * _dydx + start().y;
            }
            else {
                // TODO: x = 5 for example. Y can be anything, really...
                return 0.0;
            }
        }

        const double calculateX(const double y) const {
            if (_dy != 0) {
                return (y - start().y) * _dxdy + start().x;
            }
            else {
                // TODO: y = 5 for example. x can be anything, really...
                return 0;
            }
        }

		const Vec3d& start() const { return _data.start(); }
		const Vec3d& end() const { return _data.end(); }

    private:
        LineData _data;
        double _dx;
        double _dy;
        double _dxdy;
        double _dydx;

        void cacheCoeffs() {
            _dx = end().x - start().x;
            _dy = end().y - start().y;
            if (_dy != 0.0) {
                _dxdy = _dx / _dy;
            }
            if (_dx != 0.0) {
                _dydx = _dy / _dx;
            }
        }
    };
}