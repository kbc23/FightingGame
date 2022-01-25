#pragma once
#include "model_render.h"

class ModelRender;

class Audience : public IGameObject
{
public:
    Audience();
    ~Audience();
    bool Start() override final;
    void Update() override final;


private: // enum
    /**
     * @brief アニメーションクリップ
    */
    enum AnimationEnum
    {
        enRooting, // 応援
        AnimationMax
    };

private: //data member
    ModelRender* m_modelCharacter = nullptr; // プレイヤーキャラクターのモデル

    AnimationClip m_animationPlayer[AnimationMax]; // アニメーションクリップ

    Vector3 m_position[100];
};