#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "GameClear.h"
#include "GameOver.h"

using namespace KamataEngine;

TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
GameClear* gameClearScene = nullptr;
GameOver* gameOverScene = nullptr;

enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kGameOver,
	kGameClear,
};

// 現在シーン（型）
Scene scene = Scene::kTitle;

void ChangeScene() {

	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;

	case Scene::kGame:
		// 02_12 30枚目
		if (gameScene->IsFinished()) {
			Player* player_ = gameScene->GetPlayer();

			// ポーズメニューからリトライ or タイトル選択
			if (gameScene->IsPauseActive()) {
				if (gameScene->GetPauseSelection() == 0) {
					// リトライ
					delete gameScene;
					gameScene = new GameScene;
					gameScene->Initialize();
				}
				else if (gameScene->GetPauseSelection() == 1) {
					// タイトル戻り
					scene = Scene::kTitle;
					delete gameScene;
					gameScene = nullptr;
					titleScene = new TitleScene;
					titleScene->Initialize();
				}
			}
			// 死亡 or ゴール時の遷移
			else {
				if (player_->IsDead()) {
					scene = Scene::kGameOver;
					delete gameScene;
					gameScene = nullptr;
					gameOverScene = new GameOver;
					gameOverScene->Initialize();
				}
				else if (gameScene->IsClear()) {
					scene = Scene::kGameClear;
					delete gameScene;
					gameScene = nullptr;
					gameClearScene = new GameClear;
					gameClearScene->Initialize();
				}
			}
		}
		break;

	case Scene::kGameOver:
		if (gameOverScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}

		break;

	case Scene::kGameClear:
		if (gameClearScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameClearScene;
			gameClearScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameOver:
		gameOverScene->Draw();
		break;
	case Scene::kGameClear:
		gameClearScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"先生にばれずにさぼる");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	/*gameScene = new GameScene;
	gameScene->Initialize();*/

	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}

		// シーンごとにUpdate
		switch (scene) {
		case Scene::kTitle:
			titleScene->UpDate();
			break;
		case Scene::kGame:
			gameScene->UpDate();
			break;
		case Scene::kGameOver:
			gameOverScene->UpDate();
			break;
		case Scene::kGameClear:
			gameClearScene->UpDate();
			break;
		}
                    
		ChangeScene();

		// 描画開始
		dxCommon->PreDraw();

		DrawScene();

		// 描画終了
		dxCommon->PostDraw();
	}

	delete titleScene;
	delete gameScene;
	delete gameOverScene;
	delete gameClearScene;
	gameScene = nullptr;
	// エンジンの終了処理
	Finalize();


	return 0;
}
