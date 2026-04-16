#pragma once
#include <KamataEngine.h>
#include "UpData.h"
//#include "CameraController.h"
#include "Player.h"
#include <input/Input.h>

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

	bool IsFinished() const { return player_->IsDead(); }



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

	//KamataEngine::Model* playerModel_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	////カメラ
	KamataEngine::Camera camera_;

	// 自キャラ
	Player* player_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	UpData* upData = nullptr;

	// ポーズメニュー作成
	bool pauseMenuActive_ = false;

	// メニュー選択
	int pauseSelection_ = 0;

	bool finished_ = false;

	//CameraController* CController_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	//ばれたらカメラが近づいてくるフラグ
	bool isCaught_ = false;
	float catchTimer_ = 0.0f;

	//スコア
	int score_ = 0;

	int scoreCount_ = 100;

	/*========EVENT==========*/

	int gameTime = 10800; //ゲームプレイ時間
	int nextEventTime_ = 0;     // 次に発生する時間
	int eventCount = 3;  //イベント回数
	bool isEventActive_ = false; //イベントが発生するフラグ
	int eventTimer_ = 0; //イベント継続時間

	KamataEngine::Sprite* flashSprite_ = nullptr;
	Vector4 flashColor_ = { 1,1,1,0 }; 

	int gameTimer_ = 0;
};