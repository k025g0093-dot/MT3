#include "OBB.h"
#include <Novice.h>

void DrawOBB(
	 OBB& obb,
	 Matrix4x4& viewProjectionMatrix,
	 Matrix4x4& viewportMatrix,
	int color)
{


	Vector3 frontRightTop =AddVector3(obb.center, AddVector3(MultiplyVector3(obb.size.x * 0.5f, obb.orientations[0]), AddVector3(MultiplyVector3(obb.size.y * 0.5f, obb.orientations[1]), MultiplyVector3(obb.size.z * 0.5f, obb.orientations[2]))));
	Vector3 frontRightBottom = AddVector3(obb.center, AddVector3(MultiplyVector3(obb.size.x * 0.5f, obb.orientations[0]), AddVector3(MultiplyVector3(obb.size.y * -0.5f, obb.orientations[1]), MultiplyVector3(obb.size.z * 0.5f, obb.orientations[2]))));
	Vector3 frontLeftTop = AddVector3(obb.center, AddVector3(MultiplyVector3(obb.size.x * -0.5f, obb.orientations[0]), AddVector3(MultiplyVector3(obb.size.y * 0.5f, obb.orientations[1]), MultiplyVector3(obb.size.z * 0.5f, obb.orientations[2]))));
	Vector3 frontLeftBottom = AddVector3(obb.center, AddVector3(MultiplyVector3(obb.size.x * -0.5f, obb.orientations[0]), AddVector3(MultiplyVector3(obb.size.y * -0.5f, obb.orientations[1]), MultiplyVector3(obb.size.z * 0.5f, obb.orientations[2]))));

	Vector3 backRightTop = AddVector3(obb.center, AddVector3(MultiplyVector3(obb.size.x * 0.5f, obb.orientations[0]), AddVector3(MultiplyVector3(obb.size.y * 0.5f, obb.orientations[1]), MultiplyVector3(obb.size.z * -0.5f, obb.orientations[2]))));
	Vector3 backRightBottom = AddVector3(obb.center, AddVector3(MultiplyVector3(obb.size.x * 0.5f, obb.orientations[0]), AddVector3(MultiplyVector3(obb.size.y * -0.5f, obb.orientations[1]), MultiplyVector3(obb.size.z * -0.5f, obb.orientations[2]))));
	Vector3 backLeftTop = AddVector3(obb.center, AddVector3(MultiplyVector3(obb.size.x * -0.5f, obb.orientations[0]), AddVector3(MultiplyVector3(obb.size.y * 0.5f, obb.orientations[1]), MultiplyVector3(obb.size.z * -0.5f, obb.orientations[2]))));
	Vector3 backLeftBottom = AddVector3(obb.center, AddVector3(MultiplyVector3(obb.size.x * -0.5f, obb.orientations[0]), AddVector3(MultiplyVector3(obb.size.y * -0.5f, obb.orientations[1]), MultiplyVector3(obb.size.z * -0.5f, obb.orientations[2]))));

	//右の各四頂点
	Vector3 v0 = Transform(Transform(frontRightTop, viewProjectionMatrix), viewportMatrix);
	Vector3 v1 = Transform(Transform(frontRightBottom, viewProjectionMatrix), viewportMatrix);
	Vector3 v4 = Transform(Transform(backRightTop, viewProjectionMatrix), viewportMatrix);
	Vector3 v5 = Transform(Transform(backRightBottom, viewProjectionMatrix), viewportMatrix);

	//左の各四頂点
	Vector3 v2 = Transform(Transform(frontLeftTop, viewProjectionMatrix), viewportMatrix);
	Vector3 v3 = Transform(Transform(frontLeftBottom, viewProjectionMatrix), viewportMatrix);
	Vector3 v6 = Transform(Transform(backLeftTop, viewProjectionMatrix), viewportMatrix);
	Vector3 v7 = Transform(Transform(backLeftBottom, viewProjectionMatrix), viewportMatrix);

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
