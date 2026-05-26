#include "Segment.h"
#include <cmath>
#include <Novice.h>

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	float t = Dot(v1, v2) / Dot(v2, v2);

	Vector3 result{};
	result.x = v2.x * t;
	result.y = v2.y * t;
	result.z = v2.z * t;

	return result;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 v = SubtractVector3(point, segment.origin);
	float t = Dot(v, segment.diff) / Dot(segment.diff, segment.diff);

	t = fmaxf(0.0f, fminf(t, 1.0f));

	Vector3 result{};
	result.x = segment.origin.x + (segment.diff.x * t);
	result.y = segment.origin.y + (segment.diff.y * t);
	result.z = segment.origin.z + (segment.diff.z * t);

	return result;
}

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Vector3 end = Transform(Transform(AddVector3(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);

	Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
}