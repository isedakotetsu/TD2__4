#include "playerHandLeft.h"
#include <cmath>

void PlayerHandLeft::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position)
{
   assert(model);
   model_ = model;

   worldTransform_.Initialize();
   worldTransform_.translation_ = position;

   // 基準位置を保存（これが重要）
   basePosition_ = position;

   camera_ = camera;

}

void PlayerHandLeft::Update()
{
    // 時間を進める
    time_ += 0.016f;

    // sin波で上下移動
    float offsetY = std::sin(time_ * speed_) * amplitude_;

  
    worldTransform_.translation_.y = basePosition_.y + offsetY;

	updatetransform_->WorldTransformUpData(worldTransform_);

}

void PlayerHandLeft::Draw()
{
	model_->Draw(worldTransform_, *camera_);
}