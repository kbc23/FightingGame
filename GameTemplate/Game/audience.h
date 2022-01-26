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


private:
    void SetPosition();

    void SetRandomAnimationStartTime();

    void RandomAnimationStart();


private: // enum
    /**
     * @brief アニメーションクリップ
    */
    enum AnimationEnum
    {
        enRooting, // 応援
        AnimationMax
    };


private: // constant
    static const int m_AUDIENCE_GROUP_VERTICAL_NUM = 3; // 縦の人数
    static const int m_AUDIENCE_GROUP_BESIDE_NUM = 10; // 横の人数

    // １グループの観客の数
    static const int m_AUDIENCE_GROUP_RING_4_SIDES = m_AUDIENCE_GROUP_VERTICAL_NUM * m_AUDIENCE_GROUP_BESIDE_NUM;
    
    static const int m_AUDIENCE_NUM = m_AUDIENCE_GROUP_RING_4_SIDES * 4; // 観客の数


private: //data member
    ModelRender* m_modelCharacter = nullptr; // プレイヤーキャラクターのモデル

    AnimationClip m_animationPlayer[AnimationMax]; // アニメーションクリップ

    int m_animationStartTime[m_AUDIENCE_NUM] = { 0 }; // アニメーションが始まるまでのタイマー

    int m_animationStartTimer = 0; // アニメーションを始めるのに使用するタイマー

    bool m_flagAnimationStart[m_AUDIENCE_NUM] = { false };

    //Vector3 m_position[100];
};