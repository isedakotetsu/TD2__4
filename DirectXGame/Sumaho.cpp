#include "Sumaho.h"
#include "Math.h"

using namespace KamataEngine;

void Sumaho::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
    assert(model);
    model_ = model;

    worldTransform_.Initialize();

    // 1. スケール（少し大きくする）
    worldTransform_.scale_ = { 0.5f, 0.5f, 0.5f };
    worldTransform_.translation_ = position;
	// 3. 回転（少し傾ける）
    worldTransform_.rotation_ = { 1.4f, -0.0f, 0.0f };

    worldTransform_.matWorld_ = MakeAffineMatrix(
        worldTransform_.scale_,
        worldTransform_.rotation_,
        worldTransform_.translation_
    );
    worldTransform_.TransferMatrix();
    camera_ = camera;

}

void Sumaho::Update() {
    // 座標や回転が GameScene 側で書き換えられた後、行列を確定させる
    worldTransform_.matWorld_ = MakeAffineMatrix(
        worldTransform_.scale_,
        worldTransform_.rotation_,
        worldTransform_.translation_
    );
    worldTransform_.TransferMatrix();
}

void Sumaho::Draw()
{
    if (model_) 
    {
        model_->Draw(worldTransform_, *camera_);
    }
}