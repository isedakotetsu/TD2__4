#include "PlayerSumaho.h"

void PlayerSumaho::Initialize(Model* model, Camera* camera) {
    model_ = model;
    camera_ = camera;

    // ワールド変換の初期化
    worldTransform_.Initialize();

    // とりあえず見える位置に配置
    worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };
    worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
}

void PlayerSumaho::UpDate() {
    Input* input = Input::GetInstance();

    // スペースキーを押している間は下に隠す、離すと元の位置
    if (input->PushKey(DIK_SPACE)) {
        worldTransform_.translation_.y = -15.0f; // 隠れる位置
    } else {
        worldTransform_.translation_.y = -5.0f;  // 出ている位置
    }

    // 行列の転送を忘れずに
    worldTransform_.TransferMatrix();
}

void PlayerSumaho::Draw() {

    

    // モデルにワールド変換とカメラを渡して描画
    if (model_&&camera_) {
        model_->Draw(worldTransform_, *camera_);
    }
}