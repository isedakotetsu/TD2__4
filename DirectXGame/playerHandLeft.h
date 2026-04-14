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
	float amplitude_ = 0.2f;
	float speed_ = 10.0f;
	KamataEngine::Vector3 basePosition_;

	float stateTimer_ = 0.0f;
	float nextStateTime_ = 0.0f;
	bool isMoving_ = true;
	float currentSpeed_ = 0.0f;
};