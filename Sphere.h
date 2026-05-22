#pragma once
#include "Novice.h"
#include "Vector.h"
#include <cstdint>
#include<cmath>

constexpr auto pi = 3.14159265355f;//πの宣言

struct Sphere {
	Vector3 center;
	float radius;
};

void DrawSphere(
	const Sphere& sphere,
	Matrix4x4& viewProjectionMatrix,
	Matrix4x4& viewportMatrix,
	uint32_t color);