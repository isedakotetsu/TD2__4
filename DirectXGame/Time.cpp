#include "Time.h"

Time::~Time() {
    // スプライトの解放
    for (int i = 0; i < 4; i++) {
        delete spriteDigits_[i];
    }
}

void Time::Initialize() {
    // 0～9の画像を読み込み（ファイル名は適宜合わせてください）
    for (int i = 0; i < 10; i++) {
        char fileName[32];
        sprintf_s(fileName, "number/%d.png", i); // Resources/number/0.png ...
        textureHandles_[i] = TextureManager::Load(fileName);
    }

    // 4桁分のスプライトを生成（分2桁、秒2桁を想定）
    for (int i = 0; i < 4; i++) {
        spriteDigits_[i] = Sprite::Create(textureHandles_[0], { 0, 0 });
        spriteDigits_[i]->SetSize({ 64, 64 });
    }

    time_ = 0;
}

void Time::UpDate(int remainingTime) {
    time_ = remainingTime;

    // フレーム数を秒に変換 (60FPS想定)
    int totalSeconds = time_ / 60;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // 各桁の数値を抽出
    int displayData[4];
    displayData[0] = minutes / 10; // 分の10の位
    displayData[1] = minutes % 10; // 分の1の位
    displayData[2] = seconds / 10; // 秒の10の位
    displayData[3] = seconds % 10; // 秒の1の位

    // スプライトのテクスチャを対応する数字に切り替え
    for (int i = 0; i < 4; i++) {
        spriteDigits_[i]->SetTextureHandle(textureHandles_[displayData[i]]);
    }

    // 表示位置の設定（例：画面右上）
    float startX = 1000.0f;
    float y = 50.0f;
    for (int i = 0; i < 4; i++) {
        // 2桁目と3桁目の間に少し隙間を空ける（「：」の代わり）
        float offset = (i > 1) ? 20.0f : 0.0f;
        spriteDigits_[i]->SetPosition({ startX + (i * 50.0f) + offset, y });
    }
}

void Time::Draw() {
    // 4桁分描画
    for (int i = 0; i < 4; i++) {
        spriteDigits_[i]->Draw();
    }
}