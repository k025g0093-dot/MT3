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

// --- 💡 肩・肘・手の位置 ---
Vector3 transuLate[3]={
	{ 0.2f, 1.0f, 0.0f },
	{ 0.4f, 0.0f, 0.0f },
	{ 0.3f, 0.0f, 0.0f }
};
Vector3 rotates[3]={ 
	{ 0.0f, 0.0f, -6.8f },
	{ 0.0f, 0.0f, -1.4f },
	{ 0.0f, 0.0f, 0.0f }
};
Vector3 scales[3]={ 
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f }
};



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

		Matrix4x4 shoulderWorldMatrix = MakeAffineMatrix(
			scales[0], rotates[0], transuLate[0]);

		Matrix4x4 elbowWorldMatrix = Multiply(
			MakeAffineMatrix(scales[1], rotates[1], transuLate[1]),
			shoulderWorldMatrix);

		Matrix4x4 handWorldMatrix = Multiply(
			MakeAffineMatrix(scales[2], rotates[2], transuLate[2]),
			elbowWorldMatrix);

		Vector3 shoulderPos{ shoulderWorldMatrix.m[3][0], shoulderWorldMatrix.m[3][1], shoulderWorldMatrix.m[3][2] };
		Vector3 elbowPos{ elbowWorldMatrix.m[3][0], elbowWorldMatrix.m[3][1], elbowWorldMatrix.m[3][2] };
		Vector3 handPos{ handWorldMatrix.m[3][0], handWorldMatrix.m[3][1], handWorldMatrix.m[3][2] };

		// 💡 肩・肘・手のスフィアを作成
		Sphere sphereShoulder{ shoulderPos, 0.1f };
		Sphere sphereElbow{ elbowPos, 0.08f };
		Sphere sphereHand{ handPos, 0.06f };

#ifdef _DEBUG
		ImGui::Begin("Arm Hierarchy");

		// --- カメラの操作 ---
		if (ImGui::CollapsingHeader("Camera")) {
			ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
			ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("Arm Joints")) {
			const char* jointNames[] = { "Shoulder", "Elbow", "Hand" };
			for (int i = 0; i < 3; ++i) {
				char label[64];
				snprintf(label, sizeof(label), "%s Translate", jointNames[i]);
				ImGui::DragFloat3(label, &transuLate[i].x, 0.01f);

				snprintf(label, sizeof(label), "%s Rotate", jointNames[i]);
				ImGui::DragFloat3(label, &rotates[i].x, 0.01f);
			}
		}

		ImGui::End();
#endif // _DEBUG

		///
		/// ↑更新処理ここまで
		///

		DrawSphere(sphereShoulder, wordViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(sphereElbow, wordViewProjectionMatrix, viewportMatrix, GREEN);
		DrawSphere(sphereHand, wordViewProjectionMatrix, viewportMatrix, 0x0000FFff);

		DrawGrid(wordViewProjectionMatrix, viewportMatrix);

		// 💡 肩から肘、肘から手へ繋ぐラインを描画
		Segment shoulderToElbow{ shoulderPos, SubtractVector3(elbowPos, shoulderPos) };
		Segment elbowToHand{ elbowPos, SubtractVector3(handPos, elbowPos) };
		DrawSegment(shoulderToElbow, wordViewProjectionMatrix, viewportMatrix, WHITE);
		DrawSegment(elbowToHand, wordViewProjectionMatrix, viewportMatrix, WHITE);

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