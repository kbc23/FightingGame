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
     * @brief �A�j���[�V�����N���b�v
    */
    enum AnimationEnum
    {
        enRooting, // ����
        AnimationMax
    };


private: // constant
    static const int m_AUDIENCE_GROUP_VERTICAL_NUM = 3; // �c�̐l��
    static const int m_AUDIENCE_GROUP_BESIDE_NUM = 10; // ���̐l��

    // �P�O���[�v�̊ϋq�̐�
    static const int m_AUDIENCE_GROUP_RING_4_SIDES = m_AUDIENCE_GROUP_VERTICAL_NUM * m_AUDIENCE_GROUP_BESIDE_NUM;
    
    static const int m_AUDIENCE_NUM = m_AUDIENCE_GROUP_RING_4_SIDES * 4; // �ϋq�̐�


private: //data member
    ModelRender* m_modelCharacter = nullptr; // �v���C���[�L�����N�^�[�̃��f��

    AnimationClip m_animationPlayer[AnimationMax]; // �A�j���[�V�����N���b�v

    int m_animationStartTime[m_AUDIENCE_NUM] = { 0 }; // �A�j���[�V�������n�܂�܂ł̃^�C�}�[

    int m_animationStartTimer = 0; // �A�j���[�V�������n�߂�̂Ɏg�p����^�C�}�[

    bool m_flagAnimationStart[m_AUDIENCE_NUM] = { false };

    //Vector3 m_position[100];
};