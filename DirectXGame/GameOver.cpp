#include "GameOver.h"

using namespace KamataEngine;

GameOver::~GameOver() 
{
	delete fade_;
	delete resultSprite_;
	delete scoreDisplay_;
}

void GameOver::Initialize() 
{
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 2.0f);

	textureA_ = TextureManager::Load("backGround/high.png");
	textureB_ = TextureManager::Load("backGround/middle.png");
	textureC_ = TextureManager::Load("backGround/low.png");

	resultSprite_ = Sprite::Create(textureC_, { 0,0 });

	resultSprite_->SetSize({ 1280,720 });

	scoreDisplay_ = new Score();
	scoreDisplay_->Initialize();
	scoreDisplay_->SetPosition({ 350.0f, 250.0f });

	
	scoreDisplay_->SetSize({ 96.0f, 96.0f });
}


void GameOver::SetRank(ResultRank rank, int score)
{
	rank_ = rank;
	score_ = score;

	if (scoreDisplay_) {
		scoreDisplay_->UpDate(score_);
	}

	switch (rank_)
	{

	case ResultRank::kA:
		resultSprite_->SetTextureHandle(textureA_);
		break;

	case ResultRank::kB:
		resultSprite_->SetTextureHandle(textureB_);
		break;

	case ResultRank::kC:
		resultSprite_->SetTextureHandle(textureC_);
		break;
	}
}

void GameOver::UpDate() {
	// 02_13 27枚目
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}
}

void GameOver::Draw() { 
	Sprite::PreDraw();

	if (resultSprite_) {
		resultSprite_->Draw();
	}

	if (scoreDisplay_) {
		scoreDisplay_->Draw();
	}

	Sprite::PostDraw();
	fade_->Draw();
}