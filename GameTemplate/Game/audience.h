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
     * @brief �A�j���[�V�����N���b�v
    */
    enum AnimationEnum
    {
        enRooting, // ����
        AnimationMax
    };

private: //data member
    ModelRender* m_modelCharacter = nullptr; // �v���C���[�L�����N�^�[�̃��f��

    AnimationClip m_animationPlayer[AnimationMax]; // �A�j���[�V�����N���b�v

    Vector3 m_position[100];
};