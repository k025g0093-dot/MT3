#pragma once

struct Matrix4x4
{
	float m[4][4];
};

struct Vector3 {
	float x, y, z;
};

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 Transpose(const Matrix4x4& m);
Matrix4x4 Inverse(const Matrix4x4& m);
Matrix4x4 MakeIdentity4x4();

//平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& Vector);
//拡縮
Matrix4x4 MakeScaleMatrix(const Vector3& Vector);
//座標変換
Vector3 Transform(const Vector3& Vector, const Matrix4x4& matrix);

Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspect, float nearClip, float farClip);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Cross(const Vector3& v1, const Vector3& v2);
Vector3 SubtractVector3(const Vector3& v1, const Vector3& v2);
float Dot(const Vector3& v1, const Vector3& v2);
Vector3 AddVector3(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v1);
Vector3 MultiplyVector3(const float v1,const Vector3&v2);
Vector3 MultiplyVector3ToVector3(const Vector3& v1, const Vector3& v2);
Vector3 Normalize(Vector3 normal);

Vector3 Reflect(const Vector3& input, const Vector3& normal);

inline Vector3 operator+(const Vector3& v1, const Vector3& v2){return AddVector3(v1, v2); }
inline Vector3 operator-(const Vector3& v1, const Vector3& v2){return SubtractVector3(v1, v2); }
inline Vector3 operator*(const float s, const Vector3& v){return MultiplyVector3(s, v); }
inline Vector3 operator*(const Vector3& v, const float s){return MultiplyVector3(s, v); }
inline Vector3 operator/(const Vector3& v, const float s) { return MultiplyVector3(1.0f / s, v); }

inline Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return Add(m1, m2); }
inline Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) { return Subtract(m1, m2); }
inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return Multiply(m1, m2); }
