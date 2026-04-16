#include "Sumaho.h"
#include "Math.h"

using namespace KamataEngine;

void Sumaho::Initialize(Model* model) {
    assert(model);
    model_ = model;

    worldTransform_.Initialize();

    // 1. スケール（少し大きくする）
    worldTransform_.scale_ = { 2.0f, 2.0f, 2.0f };

    // 2. 位置（画面の右下あたりに配置）
    worldTransform_.translation_ = { 20.0f, -12.0f, 15.0f };

	// 3. 回転（少し傾ける）
    worldTransform_.rotation_ = { 1.4f, -0.0f, 0.0f };

    worldTransform_.matWorld_ = MakeAffineMatrix(
        worldTransform_.scale_,
        worldTransform_.rotation_,
        worldTransform_.translation_
    );
    worldTransform_.TransferMatrix();
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

void Sumaho::Draw(const Camera& camera) {
    if (model_) {
        model_->Draw(worldTransform_, camera);
    }
}