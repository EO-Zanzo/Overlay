#include <math.h>

#define PI 3.14159265358979323846f

class Vector2 {
public:
	float x, y;

	Vector2(void) {
		x = y = 0.0f;
	}

	Vector2(float X, float Y) {
		x = X; y = Y;
	}

	Vector2 operator-(const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}

	float Length(void) const {
		return sqrtf(x * x + y * y);
	}

	float DistTo(const Vector2& v) const {
		return (*this - v).Length();
	}
};

class Vector3 {
public:
	float x, y, z;

	Vector3(void) {
		x = y = z = 0.0f;
	}

	Vector3(float X, float Y, float Z) {
		x = X; y = Y; z = Z;
	}

	Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	float Dot(const Vector3& v) const {
		if (v.x == 0 && v.y == 0 && v.z == 0)
			return 0.0f;

		return (x * v.x + y * v.y + z * v.z);
	}
};