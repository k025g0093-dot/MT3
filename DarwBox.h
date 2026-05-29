#pragma once
#include "Vector.h"
#include<cmath>

struct AABB {
	Vector3 min;
	Vector3 max;
};


void DrawBox(
	const AABB& aabb,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color
	);

