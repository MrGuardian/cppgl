#pragma once
namespace CGL {
	struct Vertex {
		double x;
		double y;
		double z;

		Vertex() : x(0.0), y(0.0), z(0.0) {}
		Vertex(const Vertex& other) : x(other.x), y(other.y), z(other.z) {}
	};
}