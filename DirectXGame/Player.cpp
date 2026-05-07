#include "Player.h"
#include <algorithm>
#include <cstdlib>
#include <numbers>

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera, const Vector3& position) {
	assert(model);


	srand((unsigned int)time(nullptr));
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	const float kPlayerScale = 10.0f;
	worldTransform_.scale_ = { kPlayerScale, kPlayerScale, kPlayerScale };
	worldTransform_.translation_ = position;	//camera_->Initialize();
	worldTransform_.rotation_.y = 0.0f;
	/*worldTransform_.translation_.x = -2.0f;
	worldTransform_.translation_.y = -10.0f;*/
	worldTransform_.TransferMatrix();
	// 最初の待機時間
	upData = new UpData();
	idleCooldown_ = kIdleCooldownMin +
		(float(rand()) / RAND_MAX) *
		(kIdleCooldownMax - kIdleCooldownMin);
}
Vector3 Player::GetWorldPosition() const {

	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

bool Player::IsLooking() const {
	return lookState_ == LookState::kLooking ||lookState_ == LookState::kLookEnd;
}


bool Player::IsBackingWards()const {
	return lookState_ == LookState::kIdle;
}


//顔がどの角度に傾いてるのかを図る関数
float Player::GetRotationY() const {
	return worldTransform_.rotation_.y;
}

float Player::GetTranslationX()const {
	return worldTransform_.translation_.x;
}


//顔が画面外から戻ってくるのを感知するフラグ
bool Player::GetIsReturning() const {
    return isReturning_;
}

//FEVERが始まるのを感知する関数
void Player::StartFever() {
	isFever_ = true;
	feverTimer_ = 0.0f;
	startPos_ = worldTransform_.translation_;
}


bool Player::CanLook() const {
	return !isFever_ && !isCaught_;
}

void Player::UpDate() {

    float deltaTime = 1.0f / 60.0f;
    lookTimer_ += deltaTime;

    // ===== FEVER処理 =====
    if (isFever_) {

        feverTimer_ += deltaTime;

        worldTransform_.translation_.x += 1.0f;

        if (worldTransform_.translation_.x >= 170.0f) {
			worldTransform_.translation_.x = 170.0f;
        }

        if (feverTimer_ > 420.0f/60.0f) {

            isFever_ = false;

            // リセット
            isReturning_ = true;


            //振り向きの時間をランダム
            idleCooldown_ = kIdleCooldownMin +
                (float(rand()) / RAND_MAX) *
                (kIdleCooldownMax - kIdleCooldownMin);
        }

        worldTransform_.TransferMatrix();
    }

    // ===== 元の位置に戻る =====
    if (isReturning_) {

        float speed = 0.5f;

        Vector3 current = worldTransform_.translation_;
        Vector3 dir = {
            startPos_.x - current.x,
            startPos_.y - current.y,
            startPos_.z - current.z
        };

        float length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);

        if (length < 0.5f) {
            // 到着
            worldTransform_.translation_ = startPos_;
            isReturning_ = false;

            // 振り向き再開リセット
            lookState_ = LookState::kIdle;
            lookTimer_ = 0.0f;
            isLooked_ = false;
            worldTransform_.rotation_.y = 0.0f;

            idleCooldown_ = kIdleCooldownMin +
                (float(rand()) / RAND_MAX) *
                (kIdleCooldownMax - kIdleCooldownMin);
        }
        else {
            // 正規化して移動
            dir.x /= length;
            dir.y /= length;
            dir.z /= length;

            worldTransform_.translation_.x += dir.x * speed;
            worldTransform_.translation_.y += dir.y * speed;
            worldTransform_.translation_.z += dir.z * speed;
        }

        worldTransform_.TransferMatrix();
        //return;
    }

    // ===== 振り向き処理 =====
    if (CanLook()) {

        switch (lookState_) {

        case LookState::kIdle:
            if (lookTimer_ > idleCooldown_) {
                lookTimer_ = 0.0f;
                lookState_ = LookState::kLookStart;
                isLooked_ = true;
            }
            break;

        case LookState::kLookStart: {
            float t = std::clamp(lookTimer_ / kLookStartTime, 0.0f, 1.0f);

            worldTransform_.rotation_.y =
                (0.95f * std::numbers::pi_v<float> -kLookAngle) +
                kLookAngle * t;

            if (lookTimer_ >= kLookStartTime) {
                lookTimer_ = 0.0f;
                lookState_ = LookState::kLooking;
            }
        } break;

        case LookState::kLooking:
            if (lookTimer_ >= kLookingTime) {
                lookTimer_ = 0.0f;
                lookState_ = LookState::kLookEnd;
            }
            break;

        case LookState::kLookEnd: {
            float t = std::clamp(lookTimer_ / kLookEndTime, 0.0f, 1.0f);

            float startAngle = 0.95f * std::numbers::pi_v<float>;
            float endAngle = 0.0f;

            worldTransform_.rotation_.y =
                startAngle * (1.0f - t) + endAngle * t;

            if (lookTimer_ >= kLookEndTime) {
                worldTransform_.rotation_.y = 0.0f;
                lookTimer_ = 0.0f;
                lookState_ = LookState::kIdle;
                isLooked_ = false;

                idleCooldown_ = kIdleCooldownMin +
                    (float(rand()) / RAND_MAX) *
                    (kIdleCooldownMax - kIdleCooldownMin);
            }
        } break;
        }
    }

    upData->WorldTransformUpData(worldTransform_);
}

void Player::Draw()
{
	// 描画前処理（カリングモード、ブレンドモード、デプステストモードはデフォルト値）

	model_->Draw(worldTransform_, *camera_);
}