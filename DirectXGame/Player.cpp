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
	return lookState_ == LookState::kLookEnd;
}

void Player::UpDate() {

	float deltaTime = 1.0f / 60.0f;
	lookTimer_ += deltaTime;

	if (!isStopLook_) {

		switch (lookState_) {

		case LookState::kIdle:

			if (lookTimer_ > idleCooldown_) {
				lookTimer_ = 0.0f;
				lookState_ = LookState::kLookStart;
				isLooked_ = true;
			}

			break;

		case LookState::kLookStart: {

			float t = lookTimer_ / kLookStartTime;
			t = std::clamp(t, 0.0f, 1.0f);

			// 振り向き → 正面に戻る
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

			float t = lookTimer_ / kLookEndTime;
			t = std::clamp(t, 0.0f, 1.0f);

			// 正面 → 振り向く
			/*worldTransform_.rotation_.y =
				0.95f * std::numbers::pi_v<float> -
				kLookAngle * t;*/

			float startAngle = 0.95f * std::numbers::pi_v<float>; // 振り向き最大角
			float endAngle = 0.0f; // 真後ろ


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