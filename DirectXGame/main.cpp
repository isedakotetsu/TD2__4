#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "GameClear.h"
#include "GameOver.h"
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "BGM.h"
#include "Explanation.h"

using namespace KamataEngine;

TitleScene* titleScene = nullptr;
Explanation* explanationScene = nullptr;
GameScene* gameScene = nullptr;
GameClear* gameClearScene = nullptr;
GameOver* gameOverScene = nullptr;

enum class Scene {
	kUnknown = 0,
	kTitle,
	kExplanation,
	kGame,
	kGameOver,
	kGameClear,
};

BGM* bgm_ = nullptr;

uint32_t gamePlayBgmHandle_;
uint32_t titleBgmHandle_;
uint32_t explanationBgmHandle_;
uint32_t gameClearBgmHandle_1;
uint32_t gameClearBgmHandle_2;
uint32_t gameClearBgmHandle_3;

bool isExplanationBgmPlayed = false;

// 現在シーン（型）
Scene scene = Scene::kTitle;

void ChangeScene() 
{

	switch (scene)
	{
	case Scene::kTitle:
		if (!bgm_->IsPlaying()) {
			bgm_->BGMPlay(titleBgmHandle_);
		}

		if (titleScene->IsFinished()) {

			if (bgm_->IsPlaying()) {
				bgm_->BGMStop();
			}

			scene = Scene::kExplanation;

			// ← 追加
			isExplanationBgmPlayed = false;

			delete titleScene;
			titleScene = nullptr;

			explanationScene = new Explanation;
			explanationScene->Initialize();
		}
		break;

	case Scene::kExplanation:

		if (!isExplanationBgmPlayed) {

			
			Audio::GetInstance()->PlayWave(explanationBgmHandle_, false);

			isExplanationBgmPlayed = true;
		}

		if (explanationScene->IsFinished()) {

			if (bgm_->IsPlaying()) {
				bgm_->BGMStop();
			}

			scene = Scene::kGame;

			delete explanationScene;
			explanationScene = nullptr;

			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;

	case Scene::kGame:
		if (!bgm_->IsPlaying()) {
			bgm_->BGMPlay(gamePlayBgmHandle_);
		}

		if (gameScene->IsFinished()) 
		{

			if (gameScene->IsPauseActive()) {
				bgm_->BGMStop();

				if (gameScene->GetPauseSelection() == 0) {
					delete gameScene;
					gameScene = new GameScene;
					gameScene->Initialize();
				}
				else if (gameScene->GetPauseSelection() == 1) {
					scene = Scene::kTitle;
					delete gameScene;
					gameScene = nullptr;

					titleScene = new TitleScene;
					titleScene->Initialize();
				}
			}
			else {
				if (gameScene->IsDead()) {
					gameScene->CheckResultRank();

					auto rank = gameScene->GetResultRank();
					int finalScore = gameScene->GetScore();

					bgm_->BGMStop();

					if (rank == GameScene::ResultRank::kA) {
						bgm_->BGMPlay(gameClearBgmHandle_1);
					}
					else if (rank == GameScene::ResultRank::kB) {
						bgm_->BGMPlay(gameClearBgmHandle_2);
					}
					else {
						bgm_->BGMPlay(gameClearBgmHandle_3);
					}

					scene = Scene::kGameOver;
					delete gameScene;
					gameScene = nullptr;

					gameOverScene = new GameOver;
					gameOverScene->Initialize();

					gameOverScene->SetRank(
						static_cast<GameOver::ResultRank>(rank),
						finalScore
					);
				}
				else if (gameScene->IsClear()) {
					gameScene->CheckResultRank();

					auto rank = gameScene->GetResultRank();

					bgm_->BGMStop();

					if (rank == GameScene::ResultRank::kA) {
						bgm_->BGMPlay(gameClearBgmHandle_1);
					}
					else if (rank == GameScene::ResultRank::kB) {
						bgm_->BGMPlay(gameClearBgmHandle_2);
					}
					else {
						bgm_->BGMPlay(gameClearBgmHandle_3);
					}

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
			bgm_->BGMStop();

			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;

			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;

	case Scene::kGameClear:
		if (gameClearScene->IsFinished()) {
			bgm_->BGMStop();

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
	case Scene::kExplanation:
		explanationScene->Draw();
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

	gamePlayBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/gameplay.mp3");
	titleBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/title.mp3");
	gameClearBgmHandle_1 = Audio::GetInstance()->LoadWave("./BGM/gametop.mp3");
	gameClearBgmHandle_2 = Audio::GetInstance()->LoadWave("./BGM/ClearCenter.mp3");
	gameClearBgmHandle_3 = Audio::GetInstance()->LoadWave("./BGM/gamebottom.mp3");
	explanationBgmHandle_ = Audio::GetInstance()->LoadWave("./BGM/gamesetumei.mp3");
	


	bgm_ = new BGM();
	bgm_->Initialize();

	const int width = 1280;
	const int height = 720;
	const int frameSize = width * height * 4;

	std::vector<unsigned char> buffer(frameSize);

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

		case Scene::kExplanation:
			explanationScene->UpDate();
			break;

		case Scene::kGame:
			// ImGuiの開始処理
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
