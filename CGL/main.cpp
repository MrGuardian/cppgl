#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <vector>
#include <chrono>
#include "ObjReader.h"
#include <memory>

using std::vector;
using CGL::Vertex;

typedef unsigned char uchar;

struct Color {
	uchar r;
	uchar g;
	uchar b;
	uchar a;

	Color() : r(0), g(0), b(0), a(255) {}

	Color(uchar red, uchar green, uchar blue, uchar alpha) : r(red), g(green), b(blue), a(alpha) {}

	static const Color RED;
	static const Color BLACK;
	static const Color WHITE;
	static const Color INVALID;
};

const Color Color::RED = Color(255, 0, 0, 255);
const Color Color::BLACK = Color(0, 0, 0, 255);
const Color Color::WHITE = Color(255, 255, 255, 255);

class Bitmap {
public:
	Bitmap(int width, int height, const Color *clearColor = nullptr) : _width(width), _height(height) {
		_pixels = vector<Color>(_width * _height);
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

	const vector<Color>& getPixels() const {
		return _pixels;
	}

private:
	int _width;
	int _height;
	vector<Color> _pixels;

	/*
		returns valid pixel position within contiguous array of pixels or -1 if not within the bounds
	*/
	const int calculatePixelPosition(int x, int y) const {
		int pixelPosition = (_height - 1 - y) * _width + x;
		return (pixelPosition <= _pixels.size() - 1) ? pixelPosition : -1;
	}
};

void render(Bitmap &backBuffer);
void drawLine(const int x0, const int y0, const int x1, const int y1, Bitmap &bitmap, const Color &color);

std::unique_ptr<CGL::ObjReader> _model;

int main(int argc, char** argv) {
	Bitmap backBuffer(800, 800);

	_model = std::make_unique<CGL::ObjReader>("african_head.obj");	

	std::cout << "\nRendering...";
	auto frameStart = std::chrono::high_resolution_clock::now();
	render(backBuffer);
	auto frameEnd = std::chrono::high_resolution_clock::now();

	auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count();
	std::cout << "\nDone. Rendering took " << nanoseconds / 1000000 << " ms. FPS: " << 1000000000.0 / nanoseconds;

	// Save back buffer to disk
	stbi_write_png("output.png", backBuffer.width(), backBuffer.height(), 4, 
		static_cast<const void*>(backBuffer.getPixels().data()), 0);

	return 0;
}

void render(Bitmap &backBuffer) {
	// Draw the model
	for (int i = 0; i < _model->faces().size(); ++i) {
		for (int faceVertexIndex = 0; faceVertexIndex < 3; ++faceVertexIndex) {
			Vertex v0 = _model->faces()[i].vertices[faceVertexIndex];
			Vertex v1 = _model->faces()[i].vertices[(faceVertexIndex + 1) % 3];
			int x0 = (v0.x + 1.0) * backBuffer.width() * 0.5;
			int y0 = (v0.y + 1.0) * backBuffer.height() * 0.5;
			int x1 = (v1.x + 1.0) * backBuffer.width() * 0.5;
			int y1 = (v1.y + 1.0) * backBuffer.height() * 0.5;
			drawLine(x0, y0, x1, y1, backBuffer, Color::WHITE);
		}
	}
}

void drawLine(const int x0, const int y0, const int x1, const int y1, Bitmap &bitmap, const Color &color) {
	for (double delta = 0.0; delta < 1.0; delta += 0.001) {
		int x = x0 + (x1 - x0) * delta;
		int y = y0 + (y1 - y0) * delta;
		bitmap.setPixelColor(x, y, color);
	}
}