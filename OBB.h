#pragma once
#include "Vector.h"

struct OBB
{
	Vector3 center;
	Vector3 orientations[3];
	Vector3 size;
};

void DrawOBB(
	const OBB& obb,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	int color);

