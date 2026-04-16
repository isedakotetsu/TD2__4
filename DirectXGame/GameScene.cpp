#include "GameScene.h"
#include "KamataEngine.h"
#include "Player.h"
#include <filesystem> 



GameScene::~GameScene() {
	delete modelPlayer_;
	delete player_;
	delete playerHandLeft;
	delete modelPlayerHandLeft_;
	delete sprite_;
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


	playerHandLeft = new PlayerHandLeft();

	modelPlayerHandLeft_ = Model::CreateFromOBJ("playerHandLeft", true);

	Vector3 playerHandLeftPos = { -2.0f,0.5f,-4.3f };

	playerHandLeft->Initialize(modelPlayerHandLeft_, &camera_, playerHandLeftPos);

	//PCモデル
	pc_ = new PC();

	modelPc_ = Model::CreateFromOBJ("PC", true);

	Vector3 PcPos= { 0.0f,0.0f,-3.0f };

	pc_->Initialize(modelPc_, &camera_, PcPos);

	//gif動画
	//ゲームgif
	for (int i = 1; i <= 100; i++) {
		char file[256];
		sprintf_s(file, "gif_frames/frame_%03d.png", i);

		if (!std::filesystem::exists(std::string("Resources/") + file)) {
			break;
		}

		framesA_.push_back(TextureManager::Load(file));
	}

	//勉強gif
	for (int i = 1; i <= 100; i++) {
		char file[256];
		sprintf_s(file, "gif_frames2/frame2_%03d.png", i);

		if (!std::filesystem::exists(std::string("Resources/") + file)) 
		{
			break;
		}

		framesB_.push_back(TextureManager::Load(file));
	}

	// 初期状態画面
	currentFrames_ = &framesA_;
	frameCount_ = static_cast<int>(currentFrames_->size());

	if (!currentFrames_->empty()) 
	{
		sprite_ = Sprite::Create((*currentFrames_)[0], { 0, 0 });
	}

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
	playerHandLeft->Update();
	pc_->Update();

	//画面切り替え
	if (KamataEngine::Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		isGifA_ = !isGifA_;

		if (isGifA_) {
			currentFrames_ = &framesA_;

			if (!framesA_.empty()) {
				sprite_->SetTextureHandle(framesA_[0]);
			}

		} else {
			currentFrames_ = &framesB_;

			if (!framesB_.empty()) {
				sprite_->SetTextureHandle(framesB_[0]);
			}
		}

		frameIndex_ = 0;
		frameCount_ = static_cast<int>(currentFrames_->size());
	}


	static int timer = 0;
	timer++;

	if (currentFrames_ && !currentFrames_->empty()) 
	{

		if (timer % 3 == 0)
		{
			frameIndex_++;
			if (frameIndex_ >= frameCount_) 
			{
				frameIndex_ = 0;
			}
		}

		sprite_->SetTextureHandle((*currentFrames_)[frameIndex_]);
		sprite_->SetSize({ 220, 130 });
		sprite_->SetPosition({ 535, 400 });
	}
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
	Model::PreDraw();

	player_->Draw();
	playerHandLeft->Draw();
	pc_->Draw();

	Model::PostDraw();

	Sprite::PreDraw();
	sprite_->Draw();
	// フラッシュ描画
	flashSprite_->Draw();
	Sprite::PostDraw();

	

}