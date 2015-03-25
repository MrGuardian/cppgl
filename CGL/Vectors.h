#pragma once
#include <cmath>

namespace CppGL {
	template <typename T>
	class Vec3 {
	public:
		T x;
		T y;
		T z;

		Vec3() : x(0.0), y(0.0), z(0.0) { }
		Vec3(const T x, const T y, const T z) : x(x), y(y), z(z) { }
		Vec3(const Vec3& other) : x(other.x), y(other.y), z(other.z) { }

		void normalize() {
			T len = length();
			x /= len;
			y /= len;
			z /= len;
		}

		const Vec3 operator ^(const Vec3 &other) const {
			return Vec3(y*other.z - z*other.y, z*other.x - x*other.z, x*other.y - y*other.x);
		}

		const Vec3 operator - (const Vec3 &other) const {
			return Vec3(x - other.x, y - other.y, z - other.z);
		}

		const T operator * (const Vec3 &other) const {
			return x*other.x + y*other.y + z*other.z;
		}

		const T length() const {
			return std::sqrt(x*x + y*y + z*z);
		}
	};

	typedef Vec3<double> Vec3d;
}