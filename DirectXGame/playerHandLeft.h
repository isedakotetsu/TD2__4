#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include "UpData.h"
class PlayerHandLeft
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	void Update();
	void Draw();
	

private:
	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Model* model_ = nullptr;

	UpData* updatetransform_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;

	float time_ = 0.0f;
	float amplitude_ = 0.2f; // 上下の幅
	float speed_ = 10.0f;     // 動く速さ
	KamataEngine::Vector3 basePosition_;
};

