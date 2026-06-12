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

// 既存のobbの宣言の近くに、OBB専用の回転変数を追加
Vector3 obbRotate{ 0.0f, 0.0f, 0.0f };
OBB obb{
	{0.0f,0.0f,0.0f},
	{
		{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f}
	},
	{1.0f,1.0f,1.0f}
};

// 💡 スフィアから「線分」の初期データに戻す
Segment segment{
	{-0.7f,0.3f,0.0f},
	{2.0f,-0.5f,0.0f}
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


		Matrix4x4 obbRotateMatrix = Multiply(MakeRotateXMatrix(obbRotate.x), Multiply(MakeRotateYMatrix(obbRotate.y), MakeRotateZMatrix(obbRotate.z)));

		obb.orientations[0].x = obbRotateMatrix.m[0][0];
		obb.orientations[0].y = obbRotateMatrix.m[0][1];
		obb.orientations[0].z = obbRotateMatrix.m[0][2];

		obb.orientations[1].x = obbRotateMatrix.m[1][0];
		obb.orientations[1].y = obbRotateMatrix.m[1][1];
		obb.orientations[1].z = obbRotateMatrix.m[1][2];

		obb.orientations[2].x = obbRotateMatrix.m[2][0];
		obb.orientations[2].y = obbRotateMatrix.m[2][1];
		obb.orientations[2].z = obbRotateMatrix.m[2][2];

		// 💡 OBBと線分の当たり判定関数を呼び出すように変更
		if (isOBBToSegmentCollsion(obb, segment)) {
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

		// --- OBB の操作 ---
		if (ImGui::CollapsingHeader("OBB")) {
			ImGui::DragFloat3("OBB Center", &obb.center.x, 0.01f);
			ImGui::DragFloat3("OBB Rotate", &obbRotate.x, 0.01f);
			ImGui::DragFloat3("OBB Size", &obb.size.x, 0.01f);
		}

		// --- Segment の操作 ---
		// 💡 ImGuiの表示を線分用に変更（始点と差分ベクトルを動かせるように）
		if (ImGui::CollapsingHeader("Segment")) {
			ImGui::DragFloat3("Segment Origin", &segment.origin.x, 0.01f);
			ImGui::DragFloat3("Segment Diff", &segment.diff.x, 0.01f);
		}

		ImGui::End();
#endif // _DEBUG

		///
		/// ↑更新処理ここまで
		///

		// 💡 描画関数をスフィアから「線分」に変更
		DrawOBB(obb, wordViewProjectionMatrix, viewportMatrix, isClro ? RED : WHITE);
		DrawSegment(segment, wordViewProjectionMatrix, viewportMatrix, WHITE);
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