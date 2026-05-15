#include "Sphere.h"
void DrawSphere(
	const Sphere& sphere,
	Matrix4x4& viewProjectionMatrix,
	Matrix4x4& viewportMatrix,
	uint32_t color)
{

	const uint32_t kSubdivision = 16;
	const float kLonEvery = (pi * 2.0f) / float(kSubdivision); // 経度(横)の角度
	const float kLatEvery = pi / float(kSubdivision);          // 緯度(縦)の角度

	Matrix4x4 wvpVpMatrix = Multiply(viewProjectionMatrix, viewportMatrix);

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {

		float lat = -pi / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {

			float lon = lonIndex * kLonEvery;

			Vector3 a, b, c;

			a = {
				sphere.center.x + sphere.radius * cosf(lat) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(lat),
				sphere.center.z + sphere.radius * cosf(lat) * sinf(lon)
			};

			float nextLon = lon + kLonEvery;
			b = {
			   sphere.center.x + sphere.radius * cosf(lat) * cosf(nextLon),
			   sphere.center.y + sphere.radius * sinf(lat),
			   sphere.center.z + sphere.radius * cosf(lat) * sinf(nextLon)
			};

			float nextLat = lat + kLatEvery;
			c = {
			   sphere.center.x + sphere.radius * cosf(nextLat) * cosf(lon),
			   sphere.center.y + sphere.radius * sinf(nextLat),
			   sphere.center.z + sphere.radius * cosf(nextLat) * sinf(lon)
			};

			a = Transform(a, wvpVpMatrix);
			b = Transform(b, wvpVpMatrix);
			c = Transform(c, wvpVpMatrix);


			Novice::DrawLine((int)a.x, (int)a.y, (int)b.x, (int)b.y, color);
			Novice::DrawLine((int)a.x, (int)a.y, (int)c.x, (int)c.y, color);



		}

	}

}