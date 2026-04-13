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
	for (int i = 1; i <= 100; i++) {
		char file[256];
		sprintf_s(file, "gif_frames/frame_%03d.png", i);

		// デバッグ（重要）
		printf("check: %s\n", file);

		if (!std::filesystem::exists(std::string("Resources/") + file)) {
			break;
		}

		uint32_t tex = TextureManager::Load(file);
		frames_.push_back(tex);
	}

	frameCount_ = static_cast<int>(frames_.size());

	if (!frames_.empty()) {
		sprite_ = Sprite::Create(frames_[0], { 0, 0 });
		sprite_->SetSize({ 220, 130 });
		sprite_->SetPosition({ 535, 400 });
	}



	//CController_ = new CameraController();

	//CController_->Initialize(&camera_);//初期化

	//CController_->SetTarget(player_);//追従対象セット

	//CController_->Reset();//リセット

	/*CameraController::Rect cameraArea = { 12.0f,100.0f - 12.0f,0.0f,0.0f };

	CController_->SetMovableArea(cameraArea);*/

	upData = new UpData();
	//upData->WorldTransformUpData(player_->GetWorldTransform());
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
	player_->UpDate();
	playerHandLeft->Update();
	pc_->Update();

	static int timer = 0;
	timer++;

	if (!frames_.empty()) {

		if (timer % 3 == 0) {
			frameIndex_++;
			if (frameIndex_ >= frameCount_) {
				frameIndex_ = 0;
			}
		}

		sprite_->SetTextureHandle(frames_[frameIndex_]);
	}
	camera_.UpdateMatrix();
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
	Sprite::PostDraw();

}