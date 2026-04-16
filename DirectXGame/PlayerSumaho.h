#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class PlayerSumaho
{
public:
	

	void Initialize(Model* model, Camera* camera);

	void UpDate();

	void Draw();

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Camera* camera_ = nullptr;

};

