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

	enum class ResultRank {
		kC,
		kB,
		kA,
	};

	void Initialize();

	void UpDate();

	void Draw();

	bool IsFinished() const { return finished_; }

	// ランク設定
	void SetRank(ResultRank rank);

private:

	ResultRank rank_ = ResultRank::kC;

	Sprite* resultSprite_ = nullptr;

	uint32_t textureA_;
	uint32_t textureB_;
	uint32_t textureC_;

	bool finished_ = false;
	Fade* fade_ = nullptr;
	Phase phase_ = Phase::kFadeIn;
};
