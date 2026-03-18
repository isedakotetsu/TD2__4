#include "playerHandLeft.h"

void PlayerHandLeft::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position)
{
	assert(model);
	// モデル
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	camera_ = camera;


}

void PlayerHandLeft::Update()
{
	updatetransform_->WorldTransformUpData(worldTransform_);
}

void PlayerHandLeft::Draw()
{
	model_->Draw(worldTransform_, *camera_);
}