#include <Novice.h>
#include "Vector.h"
#include <cstdint>
#include<cmath>
#include "Grid.h"
#include "Sphere.h"
#ifdef _DEBUG
#include<imgui.h>
#endif
#include "Collision.h"

const char kWindowTitle[] = "LE2B_29_ヤマトユウヤ_タイトル";



static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);

		}
	}
	Novice::ScreenPrintf(x, y - kRowHeight, "%s", label);

}

#pragma region ヴェクトル型の表示関数
void vectorScreenPrintf(int x, int y, Vector3 vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.2f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.2f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.2f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}
#pragma endregion

Vector3 rotate{};
Vector3 translate{};

Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

// 頂点配列の定義（ローカル空間の三角形頂点）
Vector3 kLocalVertices[3] = {
	{ 0.0f,  1.0f, 0.0f },
	{ 1.0f, -1.0f, 0.0f },
	{-1.0f, -1.0f, 0.0f },
};

float kWindowWidth = 1280.0f;
float kWindowHeight = 720.0f;

Vector3 planePos{ 0.0f,1.0f,0.0f };
float planeRadius = 1.0f;
Plane plane{ planePos,planeRadius };

Vector3 spherePos1{ 2.0f,0.0f,0.0f };
Vector3 spherePos2{ -2.0f,0.0f,0.0f };
float radiusSphere1 = 1.0f;
float radiusSphere2 = 1.0f;

Sphere Sphere1{ spherePos1,radiusSphere1 };
Sphere Sphere2{ spherePos2,radiusSphere2 };


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);



	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	bool isClro = false;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///



		Matrix4x4 worldMatrix = MakeAffineMatrix(
			{ 1.0f,1.0f,1.0f }, rotate, translate);

		Matrix4x4 cameraMatrix = MakeAffineMatrix(
			{ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);

		Matrix4x4 viewMatrix = Inverse(cameraMatrix);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(
			0.45f, kWindowWidth / kWindowHeight, 0.1f, 100.0f);

		Matrix4x4 wordViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));

		Matrix4x4 viewportMatrix = MakeViewportMatrix(
			0.0f, 0.0f, kWindowWidth, kWindowHeight, 0.0f, 1.0f);


		

		Sphere1.center = spherePos1;
		Sphere1.radius = radiusSphere1;

		plane.normal = Normalize(planePos);
		plane.distance = planeRadius;
		if (IsCollisionPlane(Sphere1, plane)) {
			isClro = true;
		}
		else {
			isClro = false;
		}
#ifdef _DEBUG

		ImGui::Begin("Window");

		ImGui::DragFloat3("spherePos1", &spherePos1.x, 0.01f);
		ImGui::DragFloat("radiusSphere1", &radiusSphere1, 0.01f);

		ImGui::DragFloat3("spherePos2", &planePos.x, 0.01f);
		ImGui::DragFloat("radiusSphere2", &planeRadius, 0.01f);


		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);


		ImGui::End();



#endif // _DEBUG

		///
		/// ↑更新処理ここまで
		///


		DrawPlane(plane, wordViewProjectionMatrix, viewportMatrix, isClro ? RED : WHITE);
		DrawSphere(Sphere1, wordViewProjectionMatrix, viewportMatrix, isClro ? RED : WHITE);

		DrawGrid(wordViewProjectionMatrix, viewportMatrix);


		///
		/// ↓描画処理ここから
		///



		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {

			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
