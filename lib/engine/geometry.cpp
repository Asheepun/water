#include "engine/geometry.h"

//operator overloading for Vec2f
float& Vec2f::operator[] (int i){
	switch(i){
		case 0: return x;
		case 1: return y;
		default: return x;
		//default: throw "Coord index outside of vector bound";
	}
}

Vec2f Vec2f::operator+(Vec2f vIn){
	Vec2f vOut = { x + vIn.x, y + vIn.y };
	return vOut;
}
Vec2f Vec2f::operator-(Vec2f vIn){
	Vec2f vOut = { x - vIn.x, y - vIn.y };
	return vOut;
}
Vec2f Vec2f::operator*(float a){
	Vec2f vOut = { x * a, y * a };
	return vOut;
}
Vec2f Vec2f::operator/(float a){
	Vec2f vOut = { x / a, y / a };
	return vOut;
}
void Vec2f::operator+=(Vec2f vIn){
	x += vIn.x;
	y += vIn.y;
}
void Vec2f::operator-=(Vec2f vIn){
	x -= vIn.x;
	y -= vIn.y;
}
void Vec2f::operator*=(float a){
	x *= a;
	y *= a;
}
void Vec2f::operator/=(float a){
	x /= a;
	y /= a;
}

//operator overloading for Vec3f
float& Vec3f::operator[] (int i){
	switch(i){
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: return x;
		//default: throw "Coord index outside of vector bound";
	}
}

Vec3f Vec3f::operator+(Vec3f vIn){
	Vec3f vOut = { x + vIn.x, y + vIn.y, z + vIn.z };
	return vOut;
}
Vec3f Vec3f::operator-(Vec3f vIn){
	Vec3f vOut = { x - vIn.x, y - vIn.y, z - vIn.z };
	return vOut;
}
Vec3f Vec3f::operator*(float a){
	Vec3f vOut = { x * a, y * a, z * a };
	return vOut;
}
Vec3f Vec3f::operator/(float a){
	Vec3f vOut = { x / a, y / a, z / a };
	return vOut;
}
void Vec3f::operator+=(Vec3f vIn){
	x += vIn.x;
	y += vIn.y;
	z += vIn.z;
}
void Vec3f::operator-=(Vec3f vIn){
	x -= vIn.x;
	y -= vIn.y;
	z -= vIn.z;
}
void Vec3f::operator*=(float a){
	x *= a;
	y *= a;
	z *= a;
}
void Vec3f::operator/=(float a){
	x /= a;
	y /= a;
	z /= a;
}

//operator overloading for Vec4f
float& Vec4f::operator[] (int i){
	switch(i){
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: return x;
		//default: throw "Coord index outside of vector bound";
	}
}

Vec4f Vec4f::operator+(Vec4f vIn){
	Vec4f vOut = { x + vIn.x, y + vIn.y, z + vIn.z, w + vIn.w };
	return vOut;
}
Vec4f Vec4f::operator-(Vec4f vIn){
	Vec4f vOut = { x - vIn.x, y - vIn.y, z - vIn.z, w - vIn.w };
	return vOut;
}
Vec4f Vec4f::operator*(float a){
	Vec4f vOut = { x * a, y * a, z * a, w * a };
	return vOut;
}
Vec4f Vec4f::operator/(float a){
	Vec4f vOut = { x / a, y / a, z / a, w / a };
	return vOut;
}
void Vec4f::operator+=(Vec4f vIn){
	x += vIn.x;
	y += vIn.y;
	z += vIn.z;
	w += vIn.w;
}
void Vec4f::operator-=(Vec4f vIn){
	x -= vIn.x;
	y -= vIn.y;
	z -= vIn.z;
	w -= vIn.w;
}
void Vec4f::operator*=(float a){
	x *= a;
	y *= a;
	z *= a;
	w *= a;
}
void Vec4f::operator/=(float a){
	x /= a;
	y /= a;
	z /= a;
	w /= a;
}

//operator overloading for Mat2f
float *Mat2f::operator[](int i){
	return values[i];
}

//operator overloading for Mat3f
float *Mat3f::operator[] (int i){
	return values[i];
}

//operator overloading for Mat4f
float *Mat4f::operator[] (int i){
	return values[i];
}

Mat4f Mat4f::operator+ (Mat4f mIn){
	Mat4f newMatrix;
	memcpy(newMatrix.values, values, 16 * sizeof(float));

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			newMatrix.values[i][j] += mIn.values[i][j];
		}
	}

	return newMatrix;
}
Mat4f Mat4f::operator* (Mat4f mIn){
	Mat4f newMatrix;
	memset(newMatrix.values, 0, 16 * sizeof(float));

	for(int newRow = 0; newRow < 4; newRow++){
		for(int newCol = 0; newCol < 4; newCol++){
			for(int i = 0; i < 4; i++){
				newMatrix.values[newRow][newCol] += values[newRow][i] * mIn.values[i][newCol];
			}
		}
	}

	return newMatrix;
}

Mat4f Mat4f::operator* (float a){
	Mat4f newMatrix;
	memcpy(newMatrix.values, values, 16 * sizeof(float));

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			newMatrix[i][j] *= a;
		}
	}

	return newMatrix;
}

Vec4f Mat4f::operator* (Vec4f vIn){
	Vec4f newVec;

	for(int i = 0; i < 4; i++){
		newVec[i] = vIn.x * values[i][0] + vIn.y * values[i][1] + vIn.z * values[i][2] + vIn.w * values[i][3];
	}

	return newVec;
}

void Mat4f::operator*= (Mat4f vIn){
	Mat4f valueMatrix;
	memcpy(valueMatrix.values, values, 16 * sizeof(float));

	valueMatrix = vIn * valueMatrix;

	memcpy(values, valueMatrix.values, 16 * sizeof(float));
}

//PRINT FUNCTIONS
void print(Vec2f v){
	printf("{ %f, %f }\n", v.x, v.y);
}

void print(Vec3f v){
	printf("{ %f, %f, %f }\n", v.x, v.y, v.z);
}

void print(Vec4f v){
	printf("{ %f, %f, %f, %f }\n", v.x, v.y, v.z, v.w);
}

void print(Mat4f m){
	printf("{\n");
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			printf("%f, ", m[i][j]);
		}
		printf("\n");
	}
	printf("}\n");
}

//GENERAL MATH FUNCTIONS
float normalize(float x){
	if(x == 0.0){
		return 0.0;
	}
	return x / fabs(x);
}

float square(float x){
	return x * x;
}

unsigned int ___x = 1;
unsigned int ___a = 1103515245;
unsigned int ___c = 12345;
unsigned int ___m = 2147483648;

float getRandom(){

	//return (float)rand() / (float)RAND_MAX;
	___x = (___a * ___x + ___c) % ___m;
	float r = (float)___x;
	r /= (float)___m;

	return r;
	
}

void setRandomSeed(int seed){
	___x = seed;
}

bool checkEqualsFloat(float x, float y, float tolerance){
	return fabs(x - y) < tolerance;
}

int getSign(float x){
	return 1 * (x > 0) + -1 * (x < 0) + 0 * (x == 0) + 0;
}

int sign(float x){
	return 1 * (x > 0) + -1 * (x < 0) + 0 * (x == 0) + 0;
}

int min(int a, int b){
	return (a <= b) * a + (a > b) * b;
}

int max(int a, int b){
	return (a >= b) * a + (a < b) * b;
}

//VECTOR FUNCTIONS
Vec2f getVec2f(float x, float y){
	Vec2f v = { x, y };
	return v;
}

Vec2f getVec2f(float x){
	return getVec2f(x, x);
}

Vec3f getVec3f(float x, float y, float z){
	Vec3f v = { x, y, z };
	return v;
}

Vec3f getVec3f(float x){
	return getVec3f(x, x, x);
}

Vec4f getVec4f(float x, float y, float z, float w){
	Vec4f v = { x, y, z, w };
	return v;
}

Vec4f getVec4f(float a){
	Vec4f v = { a, a, a, a };
	return v;
}

Vec4f getVec4f(Vec3f inV, float a){
	Vec4f v = { inV.x, inV.y, inV.z, a };
	return v;
}

float length(Vec2f v){
	return sqrt(v.x * v.x + v.y * v.y);
}

float length(Vec3f v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float length(Vec4f v){
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

Vec2f normalize(Vec2f v){
	if(length(v) == 0.0){
		return getVec2f(0.0);
	}
	return v / length(v);
}

Vec3f normalize(Vec3f v){
	if(length(v) == 0.0){
		return getVec3f(0.0);
	}
	return v / length(v);
}

Vec4f normalize(Vec4f v){
	if(length(v) == 0.0){
		return getVec4f(0.0);
	}
	return v / length(v);
}

Vec2f floor(Vec2f v){
	return getVec2f(floor(v.x), floor(v.y));
}
Vec3f floor(Vec3f v){
	return getVec3f(floor(v.x), floor(v.y), floor(v.z));
}

float dot(Vec2f v1, Vec2f v2){
	return v1.x * v2.x + v1.y * v2.y;
}

float dot(Vec3f v1, Vec3f v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float dot(Vec4f v1, Vec4f v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

float dotSquared(Vec2f v){
	return dot(v, v);
}
float dotSquared(Vec3f v){
	return dot(v, v);
}

float lerp(float a, float b, float t){
	return a + (b - a) * fmin(1.0, fmax(t, 0.0));
}

Vec2f lerp(Vec2f v1, Vec2f v2, float t){
	return v1 + (v2 - v1) * fmin(1.0, fmax(t, 0.0));
}

Vec3f lerp(Vec3f v1, Vec3f v2, float t){
	return v1 + (v2 - v1) * fmin(1.0, fmax(t, 0.0));
}

Vec4f lerp(Vec4f v1, Vec4f v2, float t){
	return v1 + (v2 - v1) * fmin(1.0, fmax(t, 0.0));
}

float smoothStep(float x1, float x2, float t){
	return lerp(x1, x2, t * t * t * (t * (6.0 * t - 15.0) + 10.0));
}

Vec2f smoothStep(Vec2f v1, Vec2f v2, float t){
	return lerp(v1, v2, t * t * t * (t * (6.0 * t - 15.0) + 10.0));
}

Vec3f smoothStep(Vec3f v1, Vec3f v2, float t){
	return lerp(v1, v2, t * t * t * (t * (6.0 * t - 15.0) + 10.0));
}

Vec4f smoothStep(Vec4f v1, Vec4f v2, float t){
	return lerp(v1, v2, t * t * t * (t * (6.0 * t - 15.0) + 10.0));
}

Vec3f cross(Vec3f v1, Vec3f v2){
	return getVec3f(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

Vec2f round(Vec2f v){
	return getVec2f(round(v.x), round(v.y));
}

//QUATERNION FUNCTIONS

Vec4f getQuaternion(Vec3f axis, float angle){

	Vec3f normalizedAxis = normalize(axis);

	return getVec4f(
		sin(angle / 2.0) * normalizedAxis.x,
		sin(angle / 2.0) * normalizedAxis.y,
		sin(angle / 2.0) * normalizedAxis.z,
		cos(angle / 2.0)
	);

}

Vec4f normalizeQuaternion(Vec4f q){
	Vec3f axis = normalize(getVec3f(q.x, q.y, q.z));
	return getVec4f(axis.x, axis.y, axis.z, q.w);
}

Vec4f mulQuaternions(Vec4f q1, Vec4f q2){
	Mat4f matrix = {
		q2.w, q2.z, -q2.y, q2.x,
		-q2.z, q2.w, q2.x, q2.y,
		q2.y, -q2.x, q2.w, q2.z,
		-q2.x, -q2.y, -q2.z, q2.w
	};
	return matrix * q1;
}

//VEC3F FUNCTIONS

bool checkEqualsVec3f(Vec3f v1, Vec3f v2, float tolerance){
	return fabs(v1.x - v2.x) < tolerance
		&& fabs(v1.y - v2.y) < tolerance
		&& fabs(v1.z - v2.z) < tolerance;
}

//VEC4F FUNCTIONS

void Vec4f_mulByMat4f(Vec4f *v_p, Mat4f m){

	Vec4f newV = {
		v_p->x * m.values[0][0] + v_p->y * m.values[0][1] + v_p->z * m.values[0][2] + v_p->w * m.values[0][3],
		v_p->x * m.values[1][0] + v_p->y * m.values[1][1] + v_p->z * m.values[1][2] + v_p->w * m.values[1][3],
		v_p->x * m.values[2][0] + v_p->y * m.values[2][1] + v_p->z * m.values[2][2] + v_p->w * m.values[2][3],
		v_p->x * m.values[3][0] + v_p->y * m.values[3][1] + v_p->z * m.values[3][2] + v_p->w * m.values[3][3],
	};

	*v_p = newV;

}

//MAT4F FUNCTIONS

void Mat4f_log(Mat4f m){

	printf("{\n");
	for(int i = 0; i < 4; i++){
		printf("\t");
		for(int j = 0; j < 4; j++){
			printf("%f, ", m.values[i][j]);
		}
		printf("\n");
	}
	printf("}\n");

}

Vec3f mulVec3fMat4f(Vec3f v, Mat4f m, float w){
	Vec4f v4 = m * getVec4f(v.x, v.y, v.z, w);
	return getVec3f(v4.x, v4.y, v4.z);

}

/*
void Mat4f_mulByMat4f(Mat4f *m1_p, Mat4f m2){

	Mat4f newMatrix;
	memset(newMatrix.values, 0, 16 * sizeof(float));

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			for(int k = 0; k < 4; k++){
				newMatrix.values[i][j] += m1_p->values[i][k] * m2.values[k][j];
			}
		}
	}

	memcpy(m1_p->values, newMatrix.values, 16 * sizeof(float));
	
}
*/

Mat4f getIdentityMat4f(){

	Mat4f identityMat4f = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};

	return identityMat4f;

}

/*
Mat4f getRotationMat4f(float x, float y, float z){

	Mat4f xRotationMat4f = {
		1, 0, 	   0, 		0,
		0, cos(x), -sin(x),	0,
		0, sin(x), cos(x), 	0,
		0, 0, 	   0, 		1,
	};

	Mat4f yRotationMat4f = {
		cos(y),  0, sin(y), 0,
		0, 		 1, 0, 		0,
		-sin(y), 0, cos(y), 0,
		0, 		 0, 0, 		1,
	};

	Mat4f zRotationMat4f = {
		cos(z),	-sin(z), 0, 0,
		sin(z),	cos(z),  0, 0,
		0, 		0, 		 1, 0,
		0, 		0, 		 0, 1,
	};

	
	Mat4f_mulByMat4f(&xRotationMat4f, yRotationMat4f);
	Mat4f_mulByMat4f(&xRotationMat4f, zRotationMat4f);

	return xRotationMat4f;
	
}
*/

Mat4f getTranslationMat4f(float x, float y, float z){

	Mat4f translationMat4f = {
		1.0, 0.0, 0.0, x,
		0.0, 1.0, 0.0, y,
		0.0, 0.0, 1.0, z,
		0.0, 0.0, 0.0, 1.0,
	};

	return translationMat4f;

}

Mat4f getTranslationMat4f(Vec3f v){
	return getTranslationMat4f(v.x, v.y, v.z);
}

Mat4f getScalingMat4f(float x, float y, float z){

	Mat4f scalingMat4f = {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1,
	};

	return scalingMat4f;

}

Mat4f getScalingMat4f(Vec3f v){
	return getScalingMat4f(v.x, v.y, v.z);
}

Mat4f getScalingMat4f(float scale){
	return getScalingMat4f(scale, scale, scale);
}

#ifdef ENGINE_LANDSCAPE_MODE
Mat4f getPerspectiveMat4f(float fov, float aspectRatio, float far, float near){

	Mat4f perspectiveMat4f = {
		1 / (aspectRatio * tan(fov / 2)), 	0, 				  0,   0,
		0, 									1 / tan(fov / 2), 0,   0,
		0, 									0,				  (float)((far + near) / (far - near)), -(float)(2.0 * far * near / (far - near)),
		//0, 									0,				  1.0, 0.0,
		0, 									0, 				  1,   0,
	};
	
	return perspectiveMat4f;


}
#else
Mat4f getPerspectiveMat4f(float fov, float aspectRatio, float far, float near){

	Mat4f perspectiveMat4f = {
		1 / (tan(fov / 2)), 	0, 				  0,   0,
		0, 									1 / (tan(fov / 2) / aspectRatio), 0,   0,
		0, 									0,				  (float)((far + near) / (far - near)), -(float)(2.0 * far * near / (far - near)),
		//0, 									0,				  1.0, 0.0,
		0, 									0, 				  1,   0,
	};
	
	return perspectiveMat4f;


}
#endif

Mat4f getOrthographicMat4f(float scalingFactorX, float scalingFactorY, float aspectRatio, float far, float near){

	Mat4f matrix = {
		1 / (aspectRatio * scalingFactorX),  0, 				   0,							0,
		0, 				 					1 / scalingFactorY, 0,							0,
		0, 				 					0,				   (float)(1.0 / ((far - near))), 0,
		0, 				 					0, 				   0,   						1.0,
		
	};

	return matrix;

}

Mat4f getLookAtMat4f(Vec3f pos, Vec3f direction){

	Vec3f right = cross(getVec3f(0.0, 1.0, 0.0), direction);
	Vec3f up = cross(direction, right);

	right = normalize(right);
	up = normalize(up);

	Mat4f matrix = getTranslationMat4f(-pos.x, -pos.y, -pos.z);

	Mat4f lookAtMatrix = { 
		right.x,	 right.y, 	  right.z, 	   0.0,
		up.x, 	   	 up.y, 		  up.z, 	   0.0,
		direction.x, direction.y, direction.z, 0.0,
		0.0, 		 0.0,  		  0.0, 		   1.0,
	};

	/*
	Mat4f matrix = { 
		right.x,	 right.y, 	  right.z, 	   -pos.x,
		up.x, 	   	 up.y, 		  up.z, 	   -pos.y,
		direction.x, direction.y, direction.z, -pos.z,
		0.0, 		 0.0,  		  0.0, 		   1.0,
	};
	*/

	matrix *= lookAtMatrix;

	return matrix;

}

Mat4f getQuaternionMat4f(Vec4f q){

	Mat4f matrix = { 
		(float)(1.0 - 2.0 * (q.y * q.y + q.z * q.z)), (float)(2.0 * (q.x * q.y - q.z * q.w)),		(float)(2.0 * (q.x * q.z + q.y * q.w)), 0.0,
		(float)(2.0 * (q.x * q.y + q.z * q.w)), 	  (float)(1.0 - 2.0 * (q.x * q.x + q.z * q.z)), (float)(2.0 * (q.y * q.z - q.x * q.w)), 0.0,
		(float)(2.0 * (q.x * q.z - q.y * q.w)), 	  (float)(2.0 * (q.y * q.z + q.x * q.w)),		(float)(1.0 - 2.0 * (q.x * q.x + q.y * q.y)), 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	return matrix;
	
}

Vec3f getRotatedVec3f(Vec3f v, Vec3f axis, float angle){

	Vec4f v4 = getVec4f(v.x, v.y, v.z, 1.0);

	Vec4f_mulByMat4f(&v4, getQuaternionMat4f(getQuaternion(axis, angle)));

	return getVec3f(v4.x, v4.y, v4.z);

}

//MATRIX FUNCTIONS

float det(Mat2f m){
	return m.values[0][0] * m.values[1][1] - m.values[0][1] * m.values[1][0];
}

float det(Mat3f m){

	Mat2f m1 = {
		m[1][1], m[1][2],
		m[2][1], m[2][2],
	};

	Mat2f m2 = {
		m[1][0], m[1][2],
		m[2][0], m[2][2],
	};

	Mat2f m3 = {
		m[1][0], m[1][1],
		m[2][0], m[2][1],
	};

	return m[0][0] * det(m1) - m[0][1] * det(m2) + m[0][2] * det(m3);

}

float det(Vec3f v1, Vec3f v2, Vec3f v3){
	return dot(cross(v1, v2), v3);
}

float det(Mat4f m){
	
	Mat3f m1 = {
		m[1][1], m[1][2], m[1][3],
		m[2][1], m[2][2], m[2][3],
		m[3][1], m[3][2], m[3][3],
	};
	
	Mat3f m2 = {
		m[1][0], m[1][2], m[1][3],
		m[2][0], m[2][2], m[2][3],
		m[3][0], m[3][2], m[3][3],
	};
	
	Mat3f m3 = {
		m[1][0], m[1][1], m[1][3],
		m[2][0], m[2][1], m[2][3],
		m[3][0], m[3][1], m[3][3],
	};
	
	Mat3f m4 = {
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2],
		m[3][0], m[3][1], m[3][2],
	};

	return m[0][0] * det(m1) - m[0][1] * det(m2) + m[0][2] * det(m3) - m[0][3] * det(m4);

}

Mat2f getRotationMat2f(float rotation){
	
	Mat2f matrix = {
		cos(rotation), -sin(rotation),
		sin(rotation), cos(rotation),
	};

	return matrix;

}

Mat4f inverse(Mat4f m){
	
	Mat3f a00 = {
		m[1][1], m[1][2], m[1][3],
		m[2][1], m[2][2], m[2][3],
		m[3][1], m[3][2], m[3][3],
	};
	
	Mat3f a01 = {
		m[1][0], m[1][2], m[1][3],
		m[2][0], m[2][2], m[2][3],
		m[3][0], m[3][2], m[3][3],
	};
	
	Mat3f a02 = {
		m[1][0], m[1][1], m[1][3],
		m[2][0], m[2][1], m[2][3],
		m[3][0], m[3][1], m[3][3],
	};
	
	Mat3f a03 = {
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2],
		m[3][0], m[3][1], m[3][2],
	};
	
	Mat3f a10 = {
		m[0][1], m[0][2], m[0][3],
		m[2][1], m[2][2], m[2][3],
		m[3][1], m[3][2], m[3][3],
	};
	
	Mat3f a11 = {
		m[0][0], m[0][2], m[0][3],
		m[2][0], m[2][2], m[2][3],
		m[3][0], m[3][2], m[3][3],
	};
	
	Mat3f a12 = {
		m[0][0], m[0][1], m[0][3],
		m[2][0], m[2][1], m[2][3],
		m[3][0], m[3][1], m[3][3],
	};
	
	Mat3f a13 = {
		m[0][0], m[0][1], m[0][2],
		m[2][0], m[2][1], m[2][2],
		m[3][0], m[3][1], m[3][2],
	};
	
	
	Mat3f a20 = {
		m[0][1], m[0][2], m[0][3],
		m[1][1], m[1][2], m[1][3],
		m[3][1], m[3][2], m[3][3],
	};
	
	Mat3f a21 = {
		m[0][0], m[0][2], m[0][3],
		m[1][0], m[1][2], m[1][3],
		m[3][0], m[3][2], m[3][3],
	};
	
	Mat3f a22 = {
		m[0][0], m[0][1], m[0][3],
		m[1][0], m[1][1], m[1][3],
		m[3][0], m[3][1], m[3][3],
	};
	
	Mat3f a23 = {
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[3][0], m[3][1], m[3][2],
	};
	
	
	Mat3f a30 = {
		m[0][1], m[0][2], m[0][3],
		m[1][1], m[1][2], m[1][3],
		m[2][1], m[2][2], m[2][3],
	};
	
	Mat3f a31 = {
		m[0][0], m[0][2], m[0][3],
		m[1][0], m[1][2], m[1][3],
		m[2][0], m[2][2], m[2][3],
	};
	
	Mat3f a32 = {
		m[0][0], m[0][1], m[0][3],
		m[1][0], m[1][1], m[1][3],
		m[2][0], m[2][1], m[2][3],
	};
	
	Mat3f a33 = {
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2],
	};

	float d = det(m);
	
	Mat4f inverseM = {
		det(a00) / d, -det(a10) / d, det(a20) / d, -det(a30) / d,
		-det(a01) / d, det(a11) / d, -det(a21) / d, det(a31) / d,
		det(a02) / d, -det(a12) / d, det(a22) / d, -det(a32) / d,
		-det(a03) / d, det(a13) / -d, det(a23) / d, det(a33) / d,
	};

	return inverseM;
	
}

void plog(Mat4f m){
	printf("{\n");
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			printf("%f, ", m[i][j]);
		}
		printf("\n");
	}
	printf("}\n");
}

bool checkTriangleTriangleCollisionVec3f(Vec3f a1, Vec3f a2, Vec3f a3, Vec3f b1, Vec3f b2, Vec3f b3, Vec3f *output_collisionPoint){

	Vec3f aNormal = normalize(cross(a2 - a1, a3 - a1));
	Vec3f bNormal = normalize(cross(b2 - b1, b3 - b1));

	Vec3f aPoints[3];
	int n_aPoints = 0;

	float t = (dot(b1, bNormal) - dot(a1, bNormal)) / dot(a2 - a1, bNormal);
	if(t >= 0.0 && t <= 1.0){
		aPoints[n_aPoints] = a1 + (a2 - a1) * t;
		n_aPoints++;
	}

	t = (dot(b1, bNormal) - dot(a2, bNormal)) / dot(a3 - a2, bNormal);
	if(t >= 0.0 && t <= 1.0){
		aPoints[n_aPoints] = a2 + (a3 - a2) * t;
		n_aPoints++;
	}

	t = (dot(b1, bNormal) - dot(a3, bNormal)) / dot(a1 - a3, bNormal);
	if(t >= 0.0 && t <= 1.0){
		aPoints[n_aPoints] = a3 + (a1 - a3) * t;
		n_aPoints++;
	}

	Vec3f bPoints[3];
	int n_bPoints = 0;

	t = (dot(a1, aNormal) - dot(b1, aNormal)) / dot(b2 - b1, aNormal);
	if(t >= 0.0 && t <= 1.0){
		bPoints[n_bPoints] = b1 + (b2 - b1) * t;
		n_bPoints++;
	}

	t = (dot(a1, aNormal) - dot(b2, aNormal)) / dot(b3 - b2, aNormal);
	if(t >= 0.0 && t <= 1.0){
		bPoints[n_bPoints] = b2 + (b3 - b2) * t;
		n_bPoints++;
	}

	t = (dot(a1, aNormal) - dot(b3, aNormal)) / dot(b1 - b3, aNormal);
	if(t >= 0.0 && t <= 1.0){
		bPoints[n_bPoints] = b3 + (b1 - b3) * t;
		n_bPoints++;
	}

	if(n_aPoints == 2
	&& n_bPoints == 2){
		if(dot(aPoints[0] - bPoints[0], aPoints[0] - bPoints[1]) < 0.0
		|| dot(aPoints[1] - bPoints[0], aPoints[1] - bPoints[1]) < 0.0
		|| dot(bPoints[0] - aPoints[0], bPoints[0] - aPoints[1]) < 0.0
		|| dot(bPoints[1] - aPoints[0], bPoints[1] - aPoints[1]) < 0.0){

			if(output_collisionPoint != NULL){
				*output_collisionPoint = (bPoints[0] + bPoints[1]) / 2.0;
			}

			return true;
			/*
			collision.pos += (bPoints[0] + bPoints[1]) / 2.0;

			if(dot(rigidBody_p->velocity * -1.0, aNormal) > dot(rigidBody_p->velocity * -1.0, collision.normal)
			|| getMagVec3f(collision.normal) < 0.01){
				collision.normal = aNormal;
			}

			n_hits++;
			*/

		}
	}

	return false;

}

//COLOR SPACE FUNCTIONS
Vec3f RGBToHSL(Vec3f rgb){
	
	float minChannel = fmin(fmin(rgb.x, rgb.y), rgb.z);
	float maxChannel = fmax(fmax(rgb.x, rgb.y), rgb.z);

	float luminance = (float)(minChannel + maxChannel) / (2.0);

	float saturation;
	if(luminance <= 0.5){
		saturation = (maxChannel - minChannel) / (maxChannel + minChannel);
	}else{
		saturation = (maxChannel - minChannel) / (2.0 - maxChannel - minChannel);
	}

	if(fabs(minChannel - maxChannel) < 0.01){
		return getVec3f(0.0, saturation, luminance);
	}

	float hue = 0.0;
	if(fabs(maxChannel - rgb.x) < 0.01){
		hue = (rgb.y - rgb.z) / (maxChannel - minChannel);
	}
	if(fabs(maxChannel - rgb.y) < 0.01){
		hue = 2.0 + (rgb.z - rgb.x) / (maxChannel - minChannel);
	}
	if(fabs(maxChannel - rgb.z) < 0.01){
		hue = 4.0 + (rgb.x - rgb.y) / (maxChannel - minChannel);
	}

	hue *= 60.0;
	if(hue < 0.0){
		hue += 360.0;
	}

	return getVec3f(hue, saturation, luminance);

}
