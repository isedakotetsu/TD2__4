#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"先生にばれずにさぼる");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();


	// ゲームシーンのインスタンスを作成
	GameScene* gameScene = new GameScene();
	gameScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}

		// --- 2. GameSceneの更新処理を呼ぶ ---
		gameScene->UpDate();

		// 描画開始
		dxCommon->PreDraw();

		// --- 3. GameSceneの描画処理を呼ぶ ---
		gameScene->Draw();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete gameScene;
	// エンジンの終了処理
	Finalize();

	return 0;
}
