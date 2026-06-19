#include <Novice.h>
#include "Vector.h"
#include <cstdint>
#include <cmath>
#include "Grid.h"
#include "Sphere.h"
#include "Segment.h"
#include "Triangle.h"
#include "DarwBox.h"
#include "OBB.h"
#include "Lerp.h"
#ifdef _DEBUG
#include <imgui.h>
#endif
#include "Collision.h"
#include <algorithm> // std::max, std::min 用

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

// --- 💡 2次ベジェ曲線用の制御点 ---
Vector3 p0{ -1.0f, 0.0f, 0.0f };
Vector3 p1{ 0.0f, 2.0f, 0.0f };
Vector3 p2{ 1.0f, 0.0f, 0.0f };



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

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

		// 💡 各制御点の位置に配置するスフィアデータを作成（半径 0.05f）
		Sphere sphereP0{ p0, 0.05f };
		Sphere sphereP1{ p1, 0.05f };
		Sphere sphereP2{ p2, 0.05f };

#ifdef _DEBUG
		ImGui::Begin("Window");

		// --- カメラの操作 ---
		if (ImGui::CollapsingHeader("Camera")) {
			ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
			ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		}

		// --- ベジェ曲線 制御点の操作 ---
		if (ImGui::CollapsingHeader("Bezier Control Points")) {
			ImGui::DragFloat3("P0 (Start)", &p0.x, 0.01f);
			ImGui::DragFloat3("P1 (Control)", &p1.x, 0.01f);
			ImGui::DragFloat3("P2 (End)", &p2.x, 0.01f);
		}

		ImGui::End();
#endif // _DEBUG

		///
		/// ↑更新処理ここまで
		///

		// 💡 ベジェ曲線のラインを描画
		DrawBezier(p0, p1, p2, wordViewProjectionMatrix, viewportMatrix, WHITE);

		// 💡 3つの制御点をスフィアとして描画（見やすいように赤系にしています）
		DrawSphere(sphereP0, wordViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(sphereP1, wordViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(sphereP2, wordViewProjectionMatrix, viewportMatrix, RED);

		DrawGrid(wordViewProjectionMatrix, viewportMatrix);

		// 制御点同士を結ぶガイド線
		Segment side1{ p0, SubtractVector3(p1, p0) };
		Segment side2{ p1, SubtractVector3(p2, p1) };
		DrawSegment(side1, wordViewProjectionMatrix, viewportMatrix, 0x550000FF);
		DrawSegment(side2, wordViewProjectionMatrix, viewportMatrix, 0x550000FF);

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