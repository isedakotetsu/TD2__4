#include "Score.h"

Score::~Score() {
	for (int i = 0; i < kMaxDigits; i++) {
		delete spriteDigits_[i];
	}
}

void Score::Initialize() {
	// 0～9の画像を読み込み
	for (int i = 0; i < 10; i++) {
		char fileName[32];
		sprintf_s(fileName, "number/%d.png", i);
		textureHandles_[i] = TextureManager::Load(fileName);
	}

	// 6桁分のスプライトを生成
	for (int i = 0; i < kMaxDigits; i++) {
		spriteDigits_[i] = Sprite::Create(textureHandles_[0], { 0, 0 });
		spriteDigits_[i]->SetSize({ 48, 48 }); // スコアは少し小さめに
	}

	score_ = 0;
	position_ = { 50.0f, 50.0f };
}
void Score::SetPosition(const KamataEngine::Vector2& position) {
	position_ = position;
}

void Score::SetSize(const KamataEngine::Vector2& size) {
	size_ = size;
}

void Score::UpDate(int score) {
	score_ = score;

	// スコアがマイナスにならないようにガード
	if (score_ < 0) score_ = 0;

	// 各桁の数値を抽出する計算
	int tempScore = score_;
	for (int i = kMaxDigits - 1; i >= 0; i--) {
		int digit = tempScore % 10;      // 1の位を取り出す
		spriteDigits_[i]->SetTextureHandle(textureHandles_[digit]);
		tempScore /= 10;                 // 桁を右にずらす
	}

	// 表示位置の設定（左上に表示する例）
	for (int i = 0; i < kMaxDigits; i++) {
		
		spriteDigits_[i]->SetSize(size_);

		float interval = size_.x + 10.0f; 
		spriteDigits_[i]->SetPosition({ position_.x + (i * interval), position_.y });
	}
}

void Score::Draw() {
	for (int i = 0; i < kMaxDigits; i++) {
		spriteDigits_[i]->Draw();
	}
}