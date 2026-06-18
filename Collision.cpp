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


bool isAABBToSegmentCollision(
	const AABB& aabb,
	const Segment& segment) {
	float tMin = 0.0f;
	float tMax = 1.0f;
	// 1. 各軸の壁（スラブ）に到達する時間を計算
	if (segment.diff.x == 0.0f) {
		if (segment.origin.x < aabb.min.x || segment.origin.x > aabb.max.x) {
			return false;  // AABBの範囲外なら衝突しない
		}
	}
	else {
		float txMin = (aabb.min.x - segment.origin.x) / segment.diff.x;
		float txMax = (aabb.max.x - segment.origin.x) / segment.diff.x;
		if (txMin > txMax) { std::swap(txMin, txMax); }
		tMin = max(tMin, txMin);
		tMax = min(tMax, txMax);
	}

	if (segment.diff.y == 0.0f) {
		if (segment.origin.y < aabb.min.y || segment.origin.y > aabb.max.y) {
			return false;
		}
	}
	else {

		float tyMin = (aabb.min.y - segment.origin.y) / segment.diff.y;
		float tyMax = (aabb.max.y - segment.origin.y) / segment.diff.y;
		if (tyMin > tyMax) { std::swap(tyMin, tyMax); }
		tMin = max(tMin, tyMin);
		tMax = min(tMax, tyMax);
	}

	if (segment.diff.z == 0.0f) {
		if (segment.origin.z < aabb.min.z || segment.origin.z > aabb.max.z) {
			return false;
		}
	}
	else {
		float tzMin = (aabb.min.z - segment.origin.z) / segment.diff.z;
		float tzMax = (aabb.max.z - segment.origin.z) / segment.diff.z;

		if (tzMin > tzMax) { std::swap(tzMin, tzMax); }
		tMin = max(tMin, tzMin);
		tMax = min(tMax, tzMax);
	}

	if (tMin <= tMax) {
		return true;
	}

	return false;
}


bool isAABBToLineCollision(
	const AABB& aabb,
	const Line& line) {
	float tMin = 0.0f;
	float tMax = 1.0f;
	// 1. 各軸の壁（スラブ）に到達する時間を計算
	if (line.diff.x == 0.0f) {
		if (line.origin.x < aabb.min.x || line.origin.x > aabb.max.x) {
			return false;  // AABBの範囲外なら衝突しない
		}
	}
	else {
		float txMin = (aabb.min.x - line.origin.x) / line.diff.x;
		float txMax = (aabb.max.x - line.origin.x) / line.diff.x;
		if (txMin > txMax) { std::swap(txMin, txMax); }
		tMin = max(tMin, txMin);
		tMax = min(tMax, txMax);
	}

	if (line.diff.y == 0.0f) {
		if (line.origin.y < aabb.min.y || line.origin.y > aabb.max.y) {
			return false;
		}
	}
	else {

		float tyMin = (aabb.min.y - line.origin.y) / line.diff.y;
		float tyMax = (aabb.max.y - line.origin.y) / line.diff.y;
		if (tyMin > tyMax) { std::swap(tyMin, tyMax); }
		tMin = max(tMin, tyMin);
		tMax = min(tMax, tyMax);
	}

	if (line.diff.z == 0.0f) {
		if (line.origin.z < aabb.min.z || line.origin.z > aabb.max.z) {
			return false;
		}
	}
	else {
		float tzMin = (aabb.min.z - line.origin.z) / line.diff.z;
		float tzMax = (aabb.max.z - line.origin.z) / line.diff.z;

		if (tzMin > tzMax) { std::swap(tzMin, tzMax); }
		tMin = max(tMin, tzMin);
		tMax = min(tMax, tzMax);
	}

	if (tMin <= tMax) {
		return true;
	}

	return false;
}


bool isOBBToSphereCollision(
	const OBB& obb,
	const Sphere& sphere
) {

	Matrix4x4 obbWorldMatrix;

	obbWorldMatrix.m[0][0] = obb.orientations[0].x;
	obbWorldMatrix.m[0][1] = obb.orientations[0].y;
	obbWorldMatrix.m[0][2] = obb.orientations[0].z;
	obbWorldMatrix.m[0][3] = 0.0f;


	obbWorldMatrix.m[1][0] = obb.orientations[1].x;
	obbWorldMatrix.m[1][1] = obb.orientations[1].y;
	obbWorldMatrix.m[1][2] = obb.orientations[1].z;
	obbWorldMatrix.m[1][3] = 0.0f;

	// ※ Z軸の向き
	obbWorldMatrix.m[2][0] = obb.orientations[2].x;
	obbWorldMatrix.m[2][1] = obb.orientations[2].y;
	obbWorldMatrix.m[2][2] = obb.orientations[2].z;
	obbWorldMatrix.m[2][3] = 0.0f;

	// 3. 移動成分（中心座標）をハメ込む！
	obbWorldMatrix.m[3][0] = obb.center.x;
	obbWorldMatrix.m[3][1] = obb.center.y;
	obbWorldMatrix.m[3][2] = obb.center.z;
	obbWorldMatrix.m[3][3] = 1.0f; // ここは決まり文句の 1.0

	// 4. あとはこの行列の Inverse（逆行列）を使えば、100%正確なローカル空間へ！
	Vector3 centerInOBBLocalSpace = Transform(sphere.center, Inverse(obbWorldMatrix));

	Vector3 closestPoint = {
		std::clamp(centerInOBBLocalSpace.x,-obb.size.x / 2.0f,obb.size.x / 2.0f),
		std::clamp(centerInOBBLocalSpace.y,-obb.size.y / 2.0f,obb.size.y / 2.0f),
		std::clamp(centerInOBBLocalSpace.z,-obb.size.z / 2.0f,obb.size.z / 2.0f),
	};
	float distance = Length(SubtractVector3(centerInOBBLocalSpace, closestPoint));
	if (distance <= sphere.radius) {
		return true;
	}
	return false;

}

bool isOBBToSegmentCollsion(const OBB& obb, const Segment& segment)
{
	Matrix4x4 obbWorldMatrix;

	obbWorldMatrix.m[0][0] = obb.orientations[0].x;
	obbWorldMatrix.m[0][1] = obb.orientations[0].y;
	obbWorldMatrix.m[0][2] = obb.orientations[0].z;
	obbWorldMatrix.m[0][3] = 0.0f;


	obbWorldMatrix.m[1][0] = obb.orientations[1].x;
	obbWorldMatrix.m[1][1] = obb.orientations[1].y;
	obbWorldMatrix.m[1][2] = obb.orientations[1].z;
	obbWorldMatrix.m[1][3] = 0.0f;

	// ※ Z軸の向き
	obbWorldMatrix.m[2][0] = obb.orientations[2].x;
	obbWorldMatrix.m[2][1] = obb.orientations[2].y;
	obbWorldMatrix.m[2][2] = obb.orientations[2].z;
	obbWorldMatrix.m[2][3] = 0.0f;

	// 3. 移動成分（中心座標）をハメ込む！
	obbWorldMatrix.m[3][0] = obb.center.x;
	obbWorldMatrix.m[3][1] = obb.center.y;
	obbWorldMatrix.m[3][2] = obb.center.z;
	obbWorldMatrix.m[3][3] = 1.0f; // ここは決まり文句の 1.0

	// 4. あとはこの行列の Inverse（逆行列）を使えば、100%正確なローカル空間へ！
	Vector3 localOrigin = Transform(segment.origin, Inverse(obbWorldMatrix));
	Vector3 localEnd = Transform(AddVector3(segment.origin, segment.diff), Inverse(obbWorldMatrix));

	AABB localAABB{
		{-obb.size.x / 2.0f, -obb.size.y / 2.0f, -obb.size.z / 2.0f},
		{+obb.size.x / 2.0f, +obb.size.y / 2.0f, +obb.size.z / 2.0f}
	};

	Line localLine;
	localLine.origin = localOrigin;
	localLine.diff = SubtractVector3(localEnd, localOrigin);;
	return isAABBToLineCollision(localAABB, localLine);
}

bool isOBBToOBBCollision(const OBB& obb1, const OBB& obb2) {

	Vector3 axes[15];

	axes[0] = obb1.orientations[0];
	axes[1] = obb1.orientations[1];
	axes[2] = obb1.orientations[2];

	axes[3] = obb2.orientations[0];
	axes[4] = obb2.orientations[1];
	axes[5] = obb2.orientations[2];

	axes[6] = Cross(obb1.orientations[0], obb2.orientations[0]);
	axes[7] = Cross(obb1.orientations[0], obb2.orientations[1]);
	axes[8] = Cross(obb1.orientations[0], obb2.orientations[2]);
	axes[9] = Cross(obb1.orientations[1], obb2.orientations[0]);
	axes[10] = Cross(obb1.orientations[1], obb2.orientations[1]);
	axes[11] = Cross(obb1.orientations[1], obb2.orientations[2]);
	axes[12] = Cross(obb1.orientations[2], obb2.orientations[0]);
	axes[13] = Cross(obb1.orientations[2], obb2.orientations[1]);
	axes[14] = Cross(obb1.orientations[2], obb2.orientations[2]);

	for (int i = 0; i < 15; ++i) {

		if (Length(axes[i]) < 0.0001f) {
			continue;
		}

		// 💡 軸を正規化（長さを1にする）しておく
		Vector3 axis = Normalize(axes[i]);

		float L1 =
			std::abs(Dot(obb1.orientations[0], axis)) * (obb1.size.x / 2.0f) +
			std::abs(Dot(obb1.orientations[1], axis)) * (obb1.size.y / 2.0f) +
			std::abs(Dot(obb1.orientations[2], axis)) * (obb1.size.z / 2.0f);

		// OBB B の影の半径 L2
		float L2 =
			std::abs(Dot(obb2.orientations[0], axis)) * (obb2.size.x / 2.0f) +
			std::abs(Dot(obb2.orientations[1], axis)) * (obb2.size.y / 2.0f) +
			std::abs(Dot(obb2.orientations[2], axis)) * (obb2.size.z / 2.0f);

		// 軸の上での、それぞれの箱の中心の位置
		float centerA = Dot(obb1.center, axis);
		float centerB = Dot(obb2.center, axis);

		// 箱Aの影の「左端」と「右端」
		float min1 = centerA - L1;
		float max1 = centerA + L1;

		// 箱Bの影の「左端」と「右端」
		float min2 = centerB - L2;
		float max2 = centerB + L2;

		//float sumSpan = L1 + L2;
		//float longSpan = (std::max)(max1, max2) - (std::min)(min1, min2);

		if (max1 < min2 || max2 < min1) {
			return false;  // 隙間がある
		}

	}


	return true;
}
