#include "playerHandLeft.h"
#include <cmath>
#include <cstdlib> 

void PlayerHandLeft::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position)
{
    assert(model);
    model_ = model;

    worldTransform_.Initialize();
    worldTransform_.translation_ = position;

    basePosition_ = position;

    camera_ = camera;

   
    stateTimer_ = 0.0f;
    isMoving_ = true;
    nextStateTime_ = 1.0f + (rand() % 100) / 100.0f;
    currentSpeed_ = speed_;
}

void PlayerHandLeft::Update()
{
    float deltaTime = 0.016f;
    time_ += deltaTime;
    stateTimer_ += deltaTime;

    //動きの変化
    if (stateTimer_ > nextStateTime_)
    {
        stateTimer_ = 0.0f;

        isMoving_ = !isMoving_;

        if (isMoving_)
        {
            nextStateTime_ = 1.0f + (rand() % 100) / 50.0f;

            
            currentSpeed_ = speed_ + ((rand() % 10) - 5) * 0.1f;
        } 
        else
        {
            nextStateTime_ = 0.5f + (rand() % 100) / 100.0f;
        }
    }

    float offsetY = 0.0f;

    if (isMoving_)
    {
        offsetY = std::sin(time_ * currentSpeed_) * amplitude_;
    }
    else
    {
        offsetY = std::sin(time_ * 2.0f) * 0.01f;
    }

    worldTransform_.translation_.y = basePosition_.y + offsetY;

    updatetransform_->WorldTransformUpData(worldTransform_);
}

void PlayerHandLeft::Draw()
{
    model_->Draw(worldTransform_, *camera_);
}