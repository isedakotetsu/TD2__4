#include "GameOver.h"

using namespace KamataEngine;

GameOver::~GameOver() 
{
	delete fade_;
	delete resultSprite_;
}

void GameOver::Initialize() 
{
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 2.0f);

	textureC_ = TextureManager::Load("backGround/backGround.png");
	textureA_ = TextureManager::Load("backGround/backGround2.png");
	textureB_ = TextureManager::Load("backGround/backGround3.png");

	resultSprite_ = Sprite::Create(textureC_, { 0,0 });

	resultSprite_->SetSize({ 1280,720 });
}


void GameOver::SetRank(ResultRank rank)
{
	rank_ = rank;

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

	Sprite::PostDraw();
	fade_->Draw();
}