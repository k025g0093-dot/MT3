#include "Grid.h"
void DrawGrid(Matrix4x4& viewProjectionMatrix, Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);




	// Z方向に伸びる線（Xを動かしていく）
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		// 3D空間上の位置を計算 (中心から左端: -kGridHalfWidth から右端へ)
		float x = -kGridHalfWidth + (xIndex * kGridEvery);

		// 1. 3D空間の始点と終点
		Vector3 startWorld = { x, 0.0f, -kGridHalfWidth };
		Vector3 endWorld = { x, 0.0f,  kGridHalfWidth };

		// 2. スクリーン座標へ変換
		Vector3 startScreen = Transform(Transform(startWorld, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(endWorld, viewProjectionMatrix), viewportMatrix);

		// 3. 描画
		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, 0xFFFFFFFF);
	}

	// X方向に伸びる線（Zを動かしていく）
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		float z = -kGridHalfWidth + (zIndex * kGridEvery);

		Vector3 startWorld = { -kGridHalfWidth, 0.0f, z };
		Vector3 endWorld = { kGridHalfWidth, 0.0f, z };

		Vector3 startScreen = Transform(Transform(startWorld, viewProjectionMatrix), viewportMatrix);
		Vector3 endScreen = Transform(Transform(endWorld, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine((int)startScreen.x, (int)startScreen.y, (int)endScreen.x, (int)endScreen.y, 0xFFFFFFFF);
	}
}