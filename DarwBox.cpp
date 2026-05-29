#include "DarwBox.h"
#include <Novice.h>

void DrawBox(
	const AABB& aabb,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color
) {

	//表面
	const Vector3 frontRightTop = Vector3(aabb.max.x, aabb.max.y, aabb.max.z);
	const Vector3 frontRightBottmo = Vector3(aabb.max.x, aabb.min.y, aabb.max.z);
	const Vector3 frontLeftTop = Vector3(aabb.min.x, aabb.max.y, aabb.max.z);
	const Vector3 frontLeftBottmo = Vector3(aabb.min.x, aabb.min.y, aabb.max.z);

	//背面
	const Vector3 backRightTop = Vector3(aabb.max.x, aabb.max.y, aabb.min.z);
	const Vector3 backRightBottmo = Vector3(aabb.max.x, aabb.min.y, aabb.min.z);
	const Vector3 backLeftTop = Vector3(aabb.min.x, aabb.max.y, aabb.min.z);
	const Vector3 backLeftBottmo = Vector3(aabb.min.x, aabb.min.y, aabb.min.z);

	//右の各四頂点
	Vector3 v0 = Transform(Transform(frontRightTop, viewProjectionMatrix), viewportMatrix);
	Vector3 v1 = Transform(Transform(frontRightBottmo, viewProjectionMatrix), viewportMatrix);
	Vector3 v4 = Transform(Transform(backRightTop, viewProjectionMatrix), viewportMatrix);
	Vector3 v5 = Transform(Transform(backRightBottmo, viewProjectionMatrix), viewportMatrix);

	//左の各四頂点
	Vector3 v2 = Transform(Transform(frontLeftTop, viewProjectionMatrix), viewportMatrix);
	Vector3 v3 = Transform(Transform(frontLeftBottmo, viewProjectionMatrix), viewportMatrix);
	Vector3 v6 = Transform(Transform(backLeftTop, viewProjectionMatrix), viewportMatrix);
	Vector3 v7 = Transform(Transform(backLeftBottmo, viewProjectionMatrix), viewportMatrix);

	//前側
	Novice::DrawLine((int)v2.x, (int)v2.y, (int)v0.x, (int)v0.y, color);
	Novice::DrawLine((int)v0.x, (int)v0.y, (int)v1.x, (int)v1.y, color);
	Novice::DrawLine((int)v1.x, (int)v1.y, (int)v3.x, (int)v3.y, color);
	Novice::DrawLine((int)v3.x, (int)v3.y, (int)v2.x, (int)v2.y, color);

	//後ろ側
	Novice::DrawLine((int)v6.x, (int)v6.y, (int)v4.x, (int)v4.y, color);
	Novice::DrawLine((int)v4.x, (int)v4.y, (int)v5.x, (int)v5.y, color);
	Novice::DrawLine((int)v5.x, (int)v5.y, (int)v7.x, (int)v7.y, color);
	Novice::DrawLine((int)v7.x, (int)v7.y, (int)v6.x, (int)v6.y, color);

	//各面の柱
	Novice::DrawLine((int)v2.x, (int)v2.y, (int)v6.x, (int)v6.y, color);
	Novice::DrawLine((int)v0.x, (int)v0.y, (int)v4.x, (int)v4.y, color);
	Novice::DrawLine((int)v1.x, (int)v1.y, (int)v5.x, (int)v5.y, color);
	Novice::DrawLine((int)v3.x, (int)v3.y, (int)v7.x, (int)v7.y, color);

}
