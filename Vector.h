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