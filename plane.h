#pragma once
#include "Vector.h"
#include<cmath>
struct Plane {

	Vector3 normal;//法線
	float distance;//距離
};

Vector3 Perpendicular(const Vector3& vector);

void DrawPlane(
	const Plane& plane,
	Matrix4x4& viewProjectionMatrix,
	Matrix4x4& viewportMatrix,
	uint32_t color);
	
