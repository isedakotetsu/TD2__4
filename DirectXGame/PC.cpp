#include "PC.h"
#include <filesystem> 


void PC::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position)
{
	assert(model);
	// モデル
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	camera_ = camera;



}

void PC::Update()
{

	updatetransform_->WorldTransformUpData(worldTransform_);
}

void PC::Draw()
{
	model_->Draw(worldTransform_, *camera_);
}