#include "Lerp.h"
#include <Novice.h>

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
    Vector3 result{};
    result.x = v1.x + t * (v2.x - v1.x);
    result.y = v1.y + t * (v2.y - v1.y);
    result.z = v1.z + t * (v2.z - v1.z);
    return result;
}

void DrawBezier(
    const Vector3& p0, const Vector3& p1, const Vector3& p2,
    const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix,
    uint32_t color)
{
    const int kSubdivisions = 128;

    // 1つ前の点を記録しておく変数
    Vector3 previousVertex = p0;

    for (int i = 1; i <= kSubdivisions; ++i) {
        // t を 0.0 から 1.0 まで少しずつ進める
        float t = (float)i / (float)kSubdivisions;

        Vector3 p01 = Lerp(p0, p1, t); // P0 と P1 の間の Lerp
        Vector3 p12 = Lerp(p1, p2, t); // P1 と P2 の間の Lerp
        Vector3 currentVertex = Lerp(p01, p12, t); 

        Vector3 start = Transform(Transform(previousVertex, viewProjectionMatrix), viewportMatrix);
        Vector3 end = Transform(Transform(currentVertex, viewProjectionMatrix), viewportMatrix);

        Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);

        previousVertex = currentVertex;
    }
}
