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
	camera_.UpdateMatrix();

	Input* input = Input::GetInstance();
	// Spaceキー押した瞬間
	if (input->TriggerKey(DIK_SPACE)) {

		// プレイヤーが振り向いてたら
		if (player_->IsLooking()) {

			// ゲームオーバー処理
			player_->IsDead();
		}
		else {
			!player_->IsDead();
		}
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
}