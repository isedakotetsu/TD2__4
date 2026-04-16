#include "GameScene.h"
#include "KamataEngine.h"
#include "Player.h"

GameScene::~GameScene() {
	delete modelPlayer_;
	delete player_;
	player_ = nullptr;
	//delete CController_;
}

void GameScene::Initialize()
{

	model_ = Model::Create();

	worldTransform_.Initialize();

	// カメラの初期化
	camera_.Initialize();
	camera_.fovAngleY = 45.0f; // ふつうは正（エンジン仕様で負が必要なら戻してOK）
	camera_.nearZ = 0.1f;
	//カメラ位置を後ろに下げる（これ重要）
	camera_.translation_ = { 0.0f,2.0f, -10.0f }; // X=右, Y=上, Z=手前(マイナス側に引く)
	camera_.rotation_ = { 0.0f, 0.0f, 0.0f };      // X回転(ピッチ) 0.45rad ≒ 26度下向き
	camera_.UpdateMatrix();

	//debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	player_ = new Player();

	modelPlayer_ = Model::CreateFromOBJ("player2", true);

	Vector3 playerPos = { 0.0f,0.0f,40.0f };

	player_->Initialize(modelPlayer_, &camera_, playerPos);

	//CController_ = new CameraController();

	//CController_->Initialize(&camera_);//初期化

	//CController_->SetTarget(player_);//追従対象セット

	//CController_->Reset();//リセット

	/*CameraController::Rect cameraArea = { 12.0f,100.0f - 12.0f,0.0f,0.0f };

	CController_->SetMovableArea(cameraArea);*/

	upData = new UpData();
	//upData->WorldTransformUpData(player_->GetWorldTransform());

	gameTime = 10800; //ゲームプレイ時間
	gameTimer_ = 0;
	isEventActive_ = false;
	eventTimer_ = 0;
	// 最初のイベント時間をランダム設定（例：3秒〜10秒）
	nextEventTime_ = rand() % 3600 + 3200; // 180〜600フレーム
	//nextEventTime_ = rand() % 420 + 180; // 180〜600フレーム
	flashSprite_ = Sprite::Create(0, { 0, 0 });

	flashSprite_->SetSize({ 1280, 720 });
}

Vector3 GameScene::GetWorldPosition() const {

	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB GameScene::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = { worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f };
	aabb.max = { worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f };

	return aabb;
}

void GameScene::UpDate()
{

	gameTime--;

	gameTimer_++;

	player_->UpDate();
	camera_.UpdateMatrix();

	Input* input = Input::GetInstance();
	// Spaceキー押した瞬間
	if (!isEventActive_ && input->PushKey(DIK_SPACE)) {

		score_ += scoreCount_;

		// プレイヤーが振り向いてたら
		if (player_->IsLooking()) {

			//player_->SetDead();
			isCaught_ = true;
			catchTimer_ = 0.0f; // ←初期化
			player_->SetStopLook(true);
			score_ -= scoreCount_;
		}
	}


	//振り向いてる時にspaceを押すとカメラが近づく処理
	if (isCaught_) {

		catchTimer_ += 1.0f / 60.0f;

		// カメラ前進
		camera_.translation_.z += 0.5f;

		//ここで判定！！
		if (camera_.translation_.z > 20.0f) {

			player_->SetDead();  // ←ここに移動
		}
		player_->SetStopLook(true);
	}


	//イベント発生
	if (!isEventActive_ && eventCount > 0 && gameTimer_ >= nextEventTime_ && player_->IsBackingWards()) {

		isEventActive_ = true;
		eventTimer_ = 420; // 7秒
		eventCount -= 1;

		// 次のイベント時間をランダムで設定
		nextEventTime_ = gameTime + (rand() % 3600 + 360);
		//nextEventTime_ = rand() % 420 + 180; // 180〜600フレーム
	}


	if (isEventActive_) {
		eventTimer_--;

		player_->SetStopLook(true);

		if (eventTimer_ <= 0) {
			isEventActive_ = false;
			player_->SetStopLook(false);
		}

		// ランダムで色変更
		flashColor_.x = (float)(rand() % 100) / 100.0f; // R
		flashColor_.y = (float)(rand() % 100) / 100.0f; // G
		flashColor_.z = (float)(rand() % 100) / 100.0f; // B
		flashColor_.w = 0.5f; // 透明度（0〜1）

		flashSprite_->SetColor(flashColor_);
	}
	else {
		// 通常時は透明
		flashColor_ = { 1,1,1,0 };
		flashSprite_->SetColor(flashColor_);
	}

	//CController_->Updata();
}

void GameScene::Draw()
{
	//DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 修正: PreDrawの引数を正しく指定
	Model::PreDraw();

	player_->Draw();
	Model::PostDraw();

	// スプライト描画開始
	Sprite::PreDraw();

	// フラッシュ描画
	flashSprite_->Draw();

	// スプライト終了
	Sprite::PostDraw();
}