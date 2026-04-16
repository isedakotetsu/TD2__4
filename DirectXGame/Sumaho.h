#pragma once
#include <KamataEngine.h>

class Sumaho {
public:
    
    void Initialize(KamataEngine::Model* model);
    void Update();
    void Draw(const KamataEngine::Camera& camera);

    // 直接触れるように public へ移動
    KamataEngine::WorldTransform worldTransform_;
private:
    
    KamataEngine::Model* model_ = nullptr;

    // 今後の拡張用（例：操作中フラグなど）
    bool isOperating_ = false;
};