#pragma once
#include <KamataEngine.h>
#include "Fade.h"

using namespace KamataEngine;

class GameOver {
public:

	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	~GameOver();

	void Initialize();

	void UpDate();

	void Draw();

	bool IsFinished() const { return finished_; }


private:

	bool finished_ = false;
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
};
