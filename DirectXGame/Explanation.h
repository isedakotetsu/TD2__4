#pragma once
#include "UpData.h"
#include <KamataEngine.h>

class Fade;

using namespace KamataEngine;

class Explanation
{
public:
	void Initialize();
	void UpDate();
	void Draw();
	~Explanation();

	bool IsFinished() const { return finished_; }

private:

	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	bool finished_ = false;

	// 02_13 12枚目
	Fade* fade_ = nullptr;

	// 02_13 27枚目 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;
};

