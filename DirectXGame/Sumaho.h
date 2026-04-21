#pragma once
#include <KamataEngine.h>

class Sumaho {
public:
    
    void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
    void Update();
    void Draw();

    // 直接触れるように public へ移動
    KamataEngine::WorldTransform worldTransform_;
private:
    
    KamataEngine::Model* model_ = nullptr;
    KamataEngine::Camera* camera_ = nullptr;


    // 今後の拡張用（例：操作中フラグなど）
    bool isOperating_ = false;
};