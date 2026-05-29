#include <Novice.h>
#include "Vector.h"
#include <cstdint>
#include<cmath>
#include "Grid.h"
#include "Sphere.h"
#include "Segment.h"
#include "Triangle.h"
#include "DarwBox.h"
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

float kWindowWidth = 1280.0f;
float kWindowHeight = 720.0f;

AABB aabb1{
	{-0.5f,-0.5f,-0.5f},
	{0,0,0}
};
AABB aabb2{
	{0.2f,0.2f,0.2f},
	{1.0f,1.0f,1.0f}
};

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

		if (isAABBToAABBCollision(aabb1, aabb2)) {
			isClro = true;
		}
		else {
			isClro = false;
		}

#ifdef _DEBUG
		ImGui::Begin("Window");

		// --- カメラの操作 ---
		if (ImGui::CollapsingHeader("Camera")) {
			ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
			ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		}

		// --- AABB 1 の操作 ---
		if (ImGui::CollapsingHeader("AABB 1")) {
			ImGui::DragFloat3("AABB1 Min", &aabb1.min.x, 0.01f);
			ImGui::DragFloat3("AABB1 Max", &aabb1.max.x, 0.01f);
		}

		// --- AABB 2 の操作 ---
		if (ImGui::CollapsingHeader("AABB 2")) {
			ImGui::DragFloat3("AABB2 Min", &aabb2.min.x, 0.01f);
			ImGui::DragFloat3("AABB2 Max", &aabb2.max.x, 0.01f);
		}

		ImGui::End();
#endif // _DEBUG

		///
		/// ↑更新処理ここまで
		///

		DrawBox(aabb1, wordViewProjectionMatrix, viewportMatrix, isClro ? RED : WHITE);
		DrawBox(aabb2, wordViewProjectionMatrix, viewportMatrix, isClro ? RED : WHITE);
		DrawGrid(wordViewProjectionMatrix, viewportMatrix);

		///
		/// ↓描画処理ここから
		///



		///----------------
		/// ↑描画処理ここまで
		///----------------

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