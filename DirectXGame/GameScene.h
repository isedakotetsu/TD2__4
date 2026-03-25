#pragma once
#include <KamataEngine.h>
#include "UpData.h"
#include "CameraController.h"
#include "Sumaho.h"

using namespace KamataEngine;

class GameScene {
public:
	void Initialize();

	void UpDate();

	void Draw();

	~GameScene();

	Player* GetPlayer() const { return player_; } // プレイヤー取得関数を追加

	//ポーズメニュー
	bool IsPauseActive() const { return pauseMenuActive_; }
	int GetPauseSelection() const { return pauseSelection_; }

	//クリアしたことを感知する関数
	bool IsClear() const { return phase_ == Phase::kClear; }

	//ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	// AABBを取得
	AABB GetAABB();

private:
	// 02_12 4枚目 ゲームのフェーズ（型）
	enum class Phase {
		kFadeIn,  // フェードイン 02_13 28枚目で追加
		kPlay,    // ゲームプレイ
		kDeath,   // デス演出
		kFadeOut, // フェードアウト 02_13 28枚目で追加
		kClear,   // クリア
	};
	Phase phase_;

	KamataEngine::Sprite* sprite_ = nullptr;

	//////3Dモデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Model* playerModel_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	////カメラ
	KamataEngine::Camera camera_;

	KamataEngine::ModelCommon* modelCommon_ = nullptr;

	Sumaho* sumaho_ = nullptr;
	KamataEngine::Model* sumahoModel_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	UpData* upData = nullptr;

	// ポーズメニュー作成
	bool pauseMenuActive_ = false;

	// メニュー選択
	int pauseSelection_ = 0;

	CameraController* CController_ = nullptr;
};