#include "TitleScene.h"

using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete fade_;
}

void TitleScene::Initialize() 
{

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TitleScene::UpDate() 
{
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

void TitleScene::Draw() 
{
	fade_->Draw();
}