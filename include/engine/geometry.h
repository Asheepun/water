struct Vec2f{
	float x;
	float y;

	float& operator[](int);
	Vec2f operator+(Vec2f);
	Vec2f operator-(Vec2f);
	Vec2f operator*(float);
	Vec2f operator/(float);
	void operator+=(Vec2f);
	void operator-=(Vec2f);
	void operator*=(float);
	void operator/=(float);
};

struct Vec3f{
	float x;
	float y;
	float z;

	float& operator[](int);
	Vec3f operator+(Vec3f);
	Vec3f operator-(Vec3f);
	Vec3f operator*(float);
	Vec3f operator/(float);
	void operator+=(Vec3f);
	void operator-=(Vec3f);
	void operator*=(float);
	void operator/=(float);
};

struct Vec4f{
	float x;
	float y;
	float z;
	float w;

	float& operator[](int);
	Vec4f operator+(Vec4f);
	Vec4f operator-(Vec4f);
	Vec4f operator*(float);
	Vec4f operator/(float);
	void operator+=(Vec4f);
	void operator-=(Vec4f);
	void operator*=(float);
	void operator/=(float);
};

struct Mat2f{
	float values[2][2];

	float *operator[](int);
};

struct Mat3f{
	float values[3][3];

	float *operator[](int);
};

struct Mat4f{
	float values[4][4];

	float *operator[](int);
	Mat4f operator+(Mat4f);
	Mat4f operator*(Mat4f);
	Mat4f operator*(float);
	Vec4f operator*(Vec4f);
	void operator*=(Mat4f);
};

//CONSTANTS
const Mat4f IDENTITY_MAT4F = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

const Mat4f ZERO_MAT4F = {
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0
};

const Vec4f IDENTITY_QUATERNION = { 0.0, 0.0, 0.0, 1.0 };
