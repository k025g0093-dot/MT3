#include "Collision.h"

bool IsCollision(const Sphere& s1, const Sphere& s2) {

	float distnce = Length(SubtractVector3(s2.center, s1.center));

	if (distnce <= s1.radius + s2.radius) {
		return true;
	}
	return false;
}

bool IsCollisionPlane(const Sphere& s1, const Plane& p1) {

	float distance = Dot(p1.normal, s1.center) - p1.distance;
	float absDistance = std::abs(distance);
	if (absDistance <= s1.radius) {
		return true;
	}
	return false;
}

bool isLineCollisionPlame(
	const Segment& segment,
	const Plane& plane)
{

	float dot = Dot(plane.normal, segment.diff);
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;


	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}
	return false;

}



bool isTraiangleCollisionLine(
	const Triangle& tri,
	const Segment& segment)
{
	Vector3 edge01 = SubtractVector3(tri.vertices[1], tri.vertices[0]);
	Vector3 edge12 = SubtractVector3(tri.vertices[2], tri.vertices[1]);
	Vector3 edge20 = SubtractVector3(tri.vertices[0], tri.vertices[2]);

	Vector3 normal = Normalize(Cross(edge01, edge12));
	float distance = Dot(normal, tri.vertices[0]);

	Plane plane{ normal, distance };

	if (!isLineCollisionPlame(segment, plane)) {
		return false;
	}

	float t = (distance - Dot(segment.origin, normal)) / Dot(normal, segment.diff);
	Vector3 point = {
		segment.origin.x + segment.diff.x * t,
		segment.origin.y + segment.diff.y * t,
		segment.origin.z + segment.diff.z * t,
	};

	Vector3 cross01 = Cross(edge01, SubtractVector3(point, tri.vertices[0]));
	Vector3 cross12 = Cross(edge12, SubtractVector3(point, tri.vertices[1]));
	Vector3 cross20 = Cross(edge20, SubtractVector3(point, tri.vertices[2]));

	if (Dot(normal, cross01) >= 0.0f &&
		Dot(normal, cross12) >= 0.0f &&
		Dot(normal, cross20) >= 0.0f) {
		return true;
	}

	return false;
}

bool isAABBToAABBCollision(
	const AABB& aabb1,
	const AABB& aabb2
)
{

	if (
		(aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z))
	{
		return true;
	}
	return false;
}

bool isAABBToSphereCollision(
	const AABB& aabb,
	const Sphere& sphere
) {

	Vector3 closestPoint = {
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z),
	};
	float distance = Length(SubtractVector3(sphere.center, closestPoint));
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}


bool isAABBToLineCollision(
	const AABB& aabb,
	const Segment& segment) {

	// 1. 各軸の壁（スラブ）に到達する時間を計算
	float txMin = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tyMin = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float tzMin = (aabb.min.z - segment.origin.z) / segment.diff.z;

	float txMax = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float tyMax = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float tzMax = (aabb.max.z - segment.origin.z) / segment.diff.z;

	if (txMin > txMax) { std::swap(txMin, txMax); }
	if (tyMin > tyMax) { std::swap(tyMin, tyMax); }
	if (tzMin > tzMax) { std::swap(tzMin, tzMax); }

	float tMin = max(max(txMin, tyMin), tzMin);
	float tMax = min(min(txMax, tyMax), tzMax);

	if (tMin <= tMax) {
		return true;
	}
	return false;
}

