#pragma once
#include <cmath>
#include "Vector.h"

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);


void DrawBezier(
    const Vector3& p0, const Vector3& p1, const Vector3& p2,
    const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix,
    uint32_t color);