#include "Triangle.h"
#include <Novice.h>

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, int color)
{
	// 3頂点をスクリーン座標に変換
	Vector3 v0 = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	Vector3 v1 = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	Vector3 v2 = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	// 3辺を描画
	Novice::DrawLine(int(v0.x), int(v0.y), int(v1.x), int(v1.y), color);
	Novice::DrawLine(int(v1.x), int(v1.y), int(v2.x), int(v2.y), color);
	Novice::DrawLine(int(v2.x), int(v2.y), int(v0.x), int(v0.y), color);
}