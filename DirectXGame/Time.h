#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

class Time {
public:
	// デストラクタ
	~Time();

	// 初期化
	void Initialize();

	// 更新
	void UpDate(int remainingTime);

	// 描画
	void Draw();

private:
	// 0~9の数字テクスチャハンドル
	uint32_t textureHandles_[10];

	// 表示用のスプライト（分10位、分1位、秒10位、秒1位の計4つ）
	Sprite* spriteDigits_[4];

	// 残り時間（フレーム数）
	int time_;

	// 数字1つの表示サイズ（必要に応じて調整してください）
	static inline const float kDigitWidth = 64.0f;
	static inline const float kDigitHeight = 64.0f;
};