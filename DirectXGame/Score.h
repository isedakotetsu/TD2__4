#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

class Score {
public:
	// デストラクタ
	~Score();

	// 初期化
	void Initialize();

	// 更新 (現在のスコアを受け取る)
	void UpDate(int score);

	// 描画
	void Draw();

private:
	// 0~9の数字テクスチャハンドル
	uint32_t textureHandles_[10];

	// 表示用のスプライト（6桁表示を想定：000000）
	// 桁数はゲームのバランスに合わせて増やしたり減らしたりしてOKです
	static inline const int kMaxDigits = 6;
	Sprite* spriteDigits_[kMaxDigits];

	// 現在のスコア
	int score_;
};