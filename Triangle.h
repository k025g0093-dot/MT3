#pragma once
#include "Vector.h"

struct Triangle
{
	Vector3 vertices[3];
};

void DrawTriangle(
	const Triangle& triangle,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix, 
	int color);
