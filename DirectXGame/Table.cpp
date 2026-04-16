#include "Table.h"
#include <filesystem> 


void Table::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position)
{
	assert(model);
	// モデル
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;


	camera_ = camera;



}

void Table::Update()
{

	updatetransform_->WorldTransformUpData(worldTransform_);
}

void Table::Draw()
{
	model_->Draw(worldTransform_, *camera_);
}