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
#ifdef _DEBUG
#include <imgui.h>
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

// --- 💡 OBB 1 のデータ ---
Vector3 obbRotate1{ 0.0f, 0.0f, 0.0f };
OBB obb1{
	{0.0f, 0.0f, 0.0f},
	{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	},
	{0.83f, 0.26f, 0.24f}
};

// --- 💡 OBB 2 のデータ（新規追加） ---
Vector3 obbRotate2{ -0.05f, -2.49f, 0.15f };
OBB obb2{
	{0.9f, 0.66f, 0.78f},
	{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f}
	},
	{0.5f, 0.37f, 0.5f}
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

		// 💡 OBB 1 の回転行列の計算と orientations への反映
		Matrix4x4 obbRotateMatrix1 = Multiply(MakeRotateXMatrix(obbRotate1.x), Multiply(MakeRotateYMatrix(obbRotate1.y), MakeRotateZMatrix(obbRotate1.z)));
		obb1.orientations[0] = { obbRotateMatrix1.m[0][0], obbRotateMatrix1.m[0][1], obbRotateMatrix1.m[0][2] };
		obb1.orientations[1] = { obbRotateMatrix1.m[1][0], obbRotateMatrix1.m[1][1], obbRotateMatrix1.m[1][2] };
		obb1.orientations[2] = { obbRotateMatrix1.m[2][0], obbRotateMatrix1.m[2][1], obbRotateMatrix1.m[2][2] };

		// 💡 OBB 2 の回転行列の計算と orientations への反映
		Matrix4x4 obbRotateMatrix2 = Multiply(MakeRotateXMatrix(obbRotate2.x), Multiply(MakeRotateYMatrix(obbRotate2.y), MakeRotateZMatrix(obbRotate2.z)));
		obb2.orientations[0] = { obbRotateMatrix2.m[0][0], obbRotateMatrix2.m[0][1], obbRotateMatrix2.m[0][2] };
		obb2.orientations[1] = { obbRotateMatrix2.m[1][0], obbRotateMatrix2.m[1][1], obbRotateMatrix2.m[1][2] };
		obb2.orientations[2] = { obbRotateMatrix2.m[2][0], obbRotateMatrix2.m[2][1], obbRotateMatrix2.m[2][2] };

		// 💡 OBB vs OBB の当たり判定関数を呼び出す（Collision.hに作成する想定）
		if (isOBBToOBBCollision(obb1, obb2)) {
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

		// --- 💡 OBB 1 の操作 ---
		if (ImGui::CollapsingHeader("OBB 1")) {
			ImGui::DragFloat3("OBB1 Center", &obb1.center.x, 0.01f);
			ImGui::DragFloat3("OBB1 Rotate", &obbRotate1.x, 0.01f);
			ImGui::DragFloat3("OBB1 Size", &obb1.size.x, 0.01f);
		}

		// --- 💡 OBB 2 の操作（新規追加） ---
		if (ImGui::CollapsingHeader("OBB 2")) {
			ImGui::DragFloat3("OBB2 Center", &obb2.center.x, 0.01f);
			ImGui::DragFloat3("OBB2 Rotate", &obbRotate2.x, 0.01f);
			ImGui::DragFloat3("OBB2 Size", &obb2.size.x, 0.01f);
		}

		ImGui::End();
#endif // _DEBUG

		///
		/// ↑更新処理ここまで
		///

		// 💡 描画関数を両方とも OBB に変更（当たったら両方、または片方を赤くする）
		DrawOBB(obb1, wordViewProjectionMatrix, viewportMatrix, isClro ? RED : WHITE);
		DrawOBB(obb2, wordViewProjectionMatrix, viewportMatrix, isClro ? RED : WHITE);
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