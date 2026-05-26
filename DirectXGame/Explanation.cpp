#include "Explanation.h"
#include "Fade.h"

using namespace KamataEngine;

Explanation::~Explanation() {
	delete fade_;
    fade_ = nullptr;

	delete sprite_;
}

void Explanation::Initialize()
{
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	texturehandle_ = TextureManager::Load("backGround/explanation.png");

	sprite_ = Sprite::Create(texturehandle_, { 0,0 });

	sprite_->SetSize({ 1280,720 });

}

void Explanation::UpDate()
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

void Explanation::Draw()
{
	Sprite::PreDraw();

	if (sprite_) {
		sprite_->Draw();
	}
	Sprite::PostDraw();


	fade_->Draw();
}