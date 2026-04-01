#pragma once
#include "KamataEngine.h"
#include "UpData.h"
#include <cmath>
#include <ctime>

class Player {
public:

	// チラ見状態
	enum class LookState {
		kIdle,      // 待機（横向き）
		kLookStart, // 振り向き開始
		kLooking,   // 振り向いたまま静止
		kLookEnd    // 振り向きをやめる
	};

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera_, const KamataEngine::Vector3& position);

	void UpDate();

	void Draw();

	// 02_10 10枚目 ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	// getter(02_06スライド11枚目で追加)
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// 02_06スライド28枚目で追加
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }

	//振り向いたことを察知するフラグ
	bool IsLooked() const { return isLooked_; }

	bool IsLooking() const;

	//死亡フラグ
	bool IsDead() const { return isDead_; }

	void SetDead() { isDead_ = true; }
private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	UpData* upData = nullptr;

	KamataEngine::Vector3 velocity_ = {};

	// 振り向くカウントをフレーム単位で数える
	uint32_t turnFrameCount = 0;

	// フレームカウントを秒単位で管理
	uint32_t turnCount = 0;
	/*-----理想は徐々に振り向く回数と速度を早くしたい-----*/

	LookState lookState_ = LookState::kIdle;

	float lookTimer_ = 0.0f;

	static inline const float kLookAngle = 0.8f;
	static inline const float kLookStartTime = 0.2f;//向きを変えるまでの時間
	static inline const float kLookingTime = 0.5f;//見続ける時間
	static inline const float kLookEndTime = 0.3f;//元に戻るまでの時間
	static inline const float kIdleCooldownMin = 0.5f;
	static inline const float kIdleCooldownMax = 10.0f;
	static inline const float kLookCycle = 2.0f;    // 1周期あたりの時間（秒）
	static inline const float kMaxLookAngle = 0.4f; // 最大角度（ラジアン）※約23度
	float idleCooldown_ = 0.0f;//次のチラ見までの待機時間

	//bossが振り向いたか察知するフラグ
	bool isLooked_ = false;

	// デスフラグ
	bool isDead_ = false;
};
