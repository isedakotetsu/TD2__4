#pragma once
#include <KamataEngine.h>
#include "UpData.h"
#include "Player.h"
#include "playerHandLeft.h"
#include "PC.h"
#include <input/Input.h>
#include "Table.h"
#include "Time.h"
#include "Score.h"
#include "BGM.h"
#include "CameraController.h"
#include "Sumaho.h"

using namespace KamataEngine;

class GameScene {
public:

	enum class ResultRank {
		kC,
		kB,
		kA,
	};

	void Initialize();

	void UpDate();

	void Draw();

	~GameScene();

	Player* GetPlayer() const { return player_; } // プレイヤー取得関数を追加

	int GetScore() const { return score_; }

	//ポーズメニュー
	bool IsPauseActive() const { return pauseMenuActive_; }
	int GetPauseSelection() const { return pauseSelection_; }

	//クリアしたことを感知する関数
	bool IsClear() const { return phase_ == Phase::kClear; }

	bool IsFinished() const { return phase_ == Phase::kDeath || phase_ == Phase::kClear; }

	// 追加: 死亡したかどうかを判定する関数（main.cppで使用）
	bool IsDead() const { return phase_ == Phase::kDeath; }


	//ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	// AABBを取得
	AABB GetAABB();

	ResultRank GetResultRank() const { return resultRank_; }

	void CheckResultRank();

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

	

	ResultRank resultRank_ = ResultRank::kC;

	KamataEngine::Sprite* sprite_ = nullptr;
	KamataEngine::Sprite* haikei_ = nullptr;

	//3Dモデル
	KamataEngine::Model* model_ = nullptr;

	//KamataEngine::Model* playerModel_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	////カメラ
	KamataEngine::Camera camera_;

	KamataEngine::ModelCommon* modelCommon_ = nullptr;

	Sumaho* sumaho_ = nullptr;
	KamataEngine::Model* sumahoModel_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	KamataEngine::Model* modelPlayer_ = nullptr;

	//playerHandLeft
	PlayerHandLeft* playerHandLeft = nullptr;
	KamataEngine::Model* modelPlayerHandLeft_ = nullptr;

	//PCモデル
	PC* pc_ = nullptr;
	KamataEngine::Model* modelPc_ = nullptr;

	//Tableモデル
	Table* table_ = nullptr;
	KamataEngine::Model* modelTable_ = nullptr;


	
	

	UpData* upData = nullptr;

	// ポーズメニュー作成
	bool pauseMenuActive_ = false;

	// メニュー選択
	int pauseSelection_ = 0;
	bool finished_ = false;
	

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;


	std::vector<uint32_t> framesA_; // ゲームGIF
	std::vector<uint32_t> framesB_; // 勉強GIF

	std::vector<uint32_t>* currentFrames_ = nullptr;

	int frameIndex_ = 0;
	int frameCount_ = 0;
	bool isGifA_ = true;

	

	//ばれたらカメラが近づいてくるフラグ
	bool isCaught_ = false;
	float catchTimer_ = 0.0f;

	//スコア
	int score_ = 0;

	int scoreCount_ = 100;

	/*========EVENT==========*/

	int gameTime = 3600; //ゲームプレイ時間
	int nextEventTime_ = 0;     // 次に発生する時間
	int eventCount = 1;  //イベント回数
	bool isEventActive_ = false; //イベントが発生するフラグ
	int eventTimer_ = 0; //イベント継続時間

	KamataEngine::Sprite* flashSprite_ = nullptr;
	Vector4 flashColor_ = { 1,1,1,0 }; 

	int gameTimer_ = 0;

	Time* timeDisplay_ = nullptr;

	Score* scoreDisplay_ = nullptr;

	float rotY;

	BGM* bgm_ = nullptr;
	uint32_t gameClearBgmHandle_1;
	uint32_t gameClearBgmHandle_2;
	uint32_t gameClearBgmHandle_3;
	uint32_t click_;
	
};