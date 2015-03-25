#pragma once
#include "Vectors.h"

namespace CppGL {
	class LineData {
	public:
		LineData(const Vec3d &start, const Vec3d &end) : _start(start), _end(end) {}

		const Vec3d& start() const { return _start; }
		const Vec3d& end() const { return _end; }

	private:
		Vec3d _start;
		Vec3d _end;
	};
}