#pragma once
#include <KamataEngine.h>

class Player;

class CameraController {

public:
	// 矩形
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 1.0f;    // 上端
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(KamataEngine::Camera* camera);

	void Updata();

	// プレイヤーの動きに合わせて動く
	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	// 範囲制限
	void SetMovableArea(Rect area) { movableArea_ = area; }

	/// <summary>
	/// カメラを取得
	/// </summary>
	/// <returns>カメラ</returns>
	const KamataEngine::Camera& GetCamera() const { return *camera_; }

private:
	KamataEngine::Camera* camera_ = nullptr;

	Player* target_ = nullptr;

	// Math* math_ = nullptr;

	KamataEngine::Vector3 targetOffset_ = {0, 0, -30.0f};

	// カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	KamataEngine::Vector3 targetCoordinates_; // 目標座標

	// 座標補間割合
	static inline const float kInterpolationRate = 0.1f;

	// 速度掛け算
	static inline const float kVelocityBias = 20.0f;

	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};
};
