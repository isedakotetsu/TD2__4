#include "GameScene.h"
#include "KamataEngine.h"
#include "Player.h"

void GameScene::Initialize() 
{
	sumahoModel_ = Model::CreateFromOBJ("sumaho", true); // フォルダ名を指定
	sumaho_ = new Sumaho();
	sumaho_->Initialize(sumahoModel_);

	camera_.Initialize();

	camera_.translation_ = { 0.0f, 0.0f, -50.0f };

	camera_.UpdateMatrix();

	modelCommon_ = ModelCommon::GetInstance();
}

void GameScene::UpDate() 
{
    Input* input = Input::GetInstance();

    // 目標の座標と回転
    Vector3 targetPos;
    Vector3 targetRot;

    if (input->PushKey(DIK_SPACE)) {
        // 【スペース押し：手元に立てる】
        targetPos = { 10.0f, 0.0f, -5.0f };
        targetRot = { 0.2f, 0.0f, 0.0f }; // 少しだけ手前に傾ける
    } else {
        // 【通常時：机に寝かせる】
        targetPos = { 20.0f, -12.0f, 15.0f };
        targetRot = { 1.4f, -0.0f, 0.0f }; // Xを大きく(1.4f〜1.5f)するとパタンと寝ます
    }

    // 線形補間でヌルっと動かす（0.1fの部分を大きくすると速くなります）
    sumaho_->worldTransform_.translation_.x += (targetPos.x - sumaho_->worldTransform_.translation_.x) * 0.1f;
    sumaho_->worldTransform_.translation_.y += (targetPos.y - sumaho_->worldTransform_.translation_.y) * 0.1f;
    sumaho_->worldTransform_.translation_.z += (targetPos.z - sumaho_->worldTransform_.translation_.z) * 0.1f;

    sumaho_->worldTransform_.rotation_.x += (targetRot.x - sumaho_->worldTransform_.rotation_.x) * 0.1f;
    sumaho_->worldTransform_.rotation_.y += (targetRot.y - sumaho_->worldTransform_.rotation_.y) * 0.1f;

    sumaho_->Update();
    camera_.UpdateMatrix();
}

void GameScene::Draw() 
{
	

	// 修正: PreDrawの引数を省略（デフォルト引数が設定されているため）
	Model::PreDraw();

	sumaho_->Draw(camera_);

	Model::PostDraw();
}

GameScene::~GameScene() 
{
	delete sumaho_;
	sumaho_ = nullptr;
}