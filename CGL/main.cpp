#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <vector>
#include <chrono>
#include "ObjReader.h"
#include <memory>
#include <algorithm>
#include "Line.h"
#include <fstream>
#include "Bitmap.h"
#include "Color.h"

using CppGL::Vec3d;
using CppGL::Face;
using CppGL::Line;
using CppGL::Color;
using CppGL::Bitmap;

void render(Bitmap &backBuffer);
void drawLine(const Line &line, Bitmap &bitmap, const Color &color);
void drawWiredFace(const Face &triangle, Bitmap &bitmap, const Color &color);
void drawFilledFace(const Face &triangle, Bitmap &bitmap, const Color &color);
bool verticesSortByDecreasingY(const Vec3d& vert1, const Vec3d& vert2);
const Vec3d modelToScreen(const Vec3d& modelVertex, const int screenWidth, const int screenHeight);

std::unique_ptr<CppGL::ObjReader> _model;

int main(int argc, char** argv) {
    Bitmap backBuffer(1000, 1000);
    std::fstream file;
    file.open("log.txt", std::ios_base::app);

    _model = std::make_unique<CppGL::ObjReader>("african_head.obj");

    file << "\nRendering...";
    auto frameStart = std::chrono::high_resolution_clock::now();
    render(backBuffer);
    auto frameEnd = std::chrono::high_resolution_clock::now();

    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count();
    file << "\nDone. Filled " << backBuffer.pixelsSet() << " points. Rendering took " <<
        nanoseconds / 1000000 << " ms. FPS: " << 1000000000.0 / nanoseconds;
    file.close();

    // Save back buffer to disk
    stbi_write_png("output.png", backBuffer.width(), backBuffer.height(), 4,
        static_cast<const void*>(backBuffer.getPixels().data()), 0);

    return 0;
}

void render(Bitmap &backBuffer) {
    // Draw the model
    const Vec3d lightDirection(0.0, 0.0, -1.0);
    for (int i = 0; i < _model->faces().size(); ++i) {
        Face triangle(_model->faces()[i].vertices);
        Vec3d normal = (triangle.vertices[2] - triangle.vertices[0]) ^ (triangle.vertices[1] - triangle.vertices[0]);
        normal.normalize();
        const double intensity = normal * lightDirection;

        // Convert to screen coordinates
        for (auto &vertex : triangle.vertices) {
            vertex = modelToScreen(vertex, backBuffer.width(), backBuffer.height());
        }

        //drawWiredFace(triangle, backBuffer, Color::GenerateRandom());
        if (intensity > 0) {
            drawFilledFace(triangle, backBuffer, Color(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }
}

void drawLine(const Line &line, Bitmap &bitmap, const Color &color) {
    double x0 = line.start().x,
        x1 = line.end().x,
        y0 = line.start().y,
        y1 = line.end().y;

    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        steep = true;
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    // Since we are rasterizing the line from left to right, we must ensure that
    // the start x is less than end x, otherwise swap the points
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    const Line correctedLine(x0, y0, x1, y1);
    for (double x = correctedLine.start().x; x < correctedLine.end().x; ++x) {
        const double y = correctedLine.calculateY(x);
        if (steep) {

            bitmap.setPixelColor(y, x, color);
        }
        else {
            bitmap.setPixelColor(x, y, color);
        }
    }
}

void drawWiredFace(const Face &triangle, Bitmap &bitmap, const Color &color) {
    for (int faceVertexIndex = 0; faceVertexIndex < 3; ++faceVertexIndex) {
        const Line line(triangle.vertices[faceVertexIndex], triangle.vertices[(faceVertexIndex + 1) % 3]);
        drawLine(line, bitmap, color);
    }
}

const Vec3d modelToScreen(const Vec3d& modelVertex, const int screenWidth, const int screenHeight) {
    Vec3d screenSpaceVertex;
    screenSpaceVertex.x = (int)((modelVertex.x + 1.0) * screenWidth * 0.5);
    screenSpaceVertex.y = (int)((modelVertex.y + 1.0) * screenHeight * 0.5);
    return screenSpaceVertex;
}

void drawFilledFace(const Face &triangle, Bitmap &bitmap, const Color &color) {
    vector<Vec3d> vertices(triangle.vertices);
    std::sort(vertices.begin(), vertices.end(), verticesSortByDecreasingY);

    if (vertices[0].y != vertices[1].y) {
        const Line line1(vertices[0], vertices[1]);
        const Line line2(vertices[0], vertices[2]);
        // Draw top-down
        for (double y = vertices[0].y; y >= vertices[1].y && y >= vertices[2].y; --y) {
            const Line lineToDraw(line1.calculateX(y), y, line2.calculateX(y), y);
            drawLine(lineToDraw, bitmap, color);
        }
        // Draw bottom-top
        if (vertices[2].y != vertices[1].y) {
            const Line line3(vertices[2], vertices[1]);
            const Line line4(vertices[2], vertices[0]);
            for (double y = vertices[2].y; y <= vertices[1].y && y <= vertices[0].y; ++y) {
                const Line lineToDraw(line3.calculateX(y), y, line4.calculateX(y), y);
                drawLine(lineToDraw, bitmap, color);
            }
        }
    }
    else {
        // Draw bottom-top		
        const Line line1(vertices[2], vertices[1]);
        const Line line2(vertices[2], vertices[0]);
        for (double y = vertices[2].y; y <= vertices[1].y && y <= vertices[0].y; ++y) {
            const Line lineToDraw(line1.calculateX(y), y, line2.calculateX(y), y);
            drawLine(lineToDraw, bitmap, color);
        }
    }
}

bool verticesSortByDecreasingY(const Vec3d& vert1, const Vec3d& vert2) {
    return vert1.y > vert2.y;
}