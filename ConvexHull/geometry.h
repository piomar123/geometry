#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <algorithm>
#include <cmath>

template<class T>
class Vector3T {
public:
	Vector3T() : x(0), y(0), z(0) {}
	Vector3T(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}
	Vector3T(const Vector3T<T>& other) : x(other.x), y(other.y), z(other.z) {}
	Vector3T<T>& operator= (const Vector3T<T>& other) { 
		x = other.x; y = other.y; z = other.z; return *this; 
	}
	Vector3T<T> operator- (const Vector3T<T>& other) const { 
		return Vector3T<T>(x - other.x, y - other.y, z - other.z); 
	}
	Vector3T<T> operator- () const { 
		return Vector3T<T>(-x, -y, -z); 
	}

	Vector3T<T> cross(const Vector3T<T>& p) const {
		return Vector3T<T>(y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);
	}

	void normalize() {
		double length = x * x + y * y + z * z;
		if (length == 0) return;
		length = std::sqrt(length);

		x /= length;
		y /= length;
		z /= length;
	}

	const T* asArray() const {
		return (T*) &x;
	}

	T x, y, z;
};

template<class T>
class Vector2T {
public:
	Vector2T() : x(0), y(0) {}
	Vector2T(T nx, T ny) : x(nx), y(ny) {}
	Vector2T(const Vector2T<T>& other) : x(other.x), y(other.y) {}
	Vector2T(const Vector2T<T>& from, const Vector2T<T>& to) : x(to.x - from.x), y(to.y - from.y) {}
	Vector2T<T>& operator= (const Vector2T<T>& other) { 
		x = other.x;
		y = other.y;
		return *this; 
	}
	Vector2T<T> operator- (const Vector2T<T>& other) const { 
		return Vector2T<T>(x - other.x, y - other.y); 
	}
	bool operator< (const Vector2T<T>& other) const {
		if (x == other.x) {
			return y < other.y;
		}
		return x < other.x;
	}

	T x, y;
};

typedef Vector3T<double> Vector3d;
typedef Vector2T<int> Vector2i;
typedef Vector2T<double> Vector2d;

template<class T>
T xClamp(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

#endif //GEOMETRY_H
