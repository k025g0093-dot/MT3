#pragma once
#include "Vector.h"
#include "Sphere.h"


struct Segment
{
	Vector3 origin;
	Vector3 diff;
};

Vector3 Project(const Vector3& v1, const Vector3& v2);

Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);