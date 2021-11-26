#pragma once
#include "model_render.h"

/**
 * @brief �L�����N�^�[�̃��f���֘A�̏���
*/
class Actor : public IGameObject
{
public:
    Actor();
    ~Actor();
    bool Start() override;
    void Update() override;

    /**
     * @brief ������
     * @param filePath ���f���̃t�@�C���p�X
     * @param initPos �����ʒu
     * @param initRotAngle ��������
     * @param pOtherActor ����̃��f��
    */
    void Init(
        const char* filePath,
        const Vector3& initPos,
        float initRotAngle
    );

    /**
     * @brief 
     * @param filePath ���f���̃t�@�C���p�X
     * @param playerNum �v���C���[�ԍ�
     * @param initPos �����ʒu
     * @param initRot ��������
    */
    void DebugInit(const char* filePath, const Vector3& initPos, const float initRot);

    /**
     * @brief ���������ɃX�e�[�^�X���X�V����
     * @param addMoveAmount �X�V����ړ���
     * @param addRotAngle �X�V�����]��
    */
    void AddStatus(Vector3& addMoveAmount);

    /**
     * @brief �L�����N�^�[���ړ������Ɍ�������
     * @param addMoveAmount �X�V����ړ���
    */
    void Turn(Vector3& addMoveAmount);


private:
    void AttackAnimation();

    /**
     * @brief �L�����N�^�[���f���ɕK�v�ȃX�e�[�^�X����n��
    */
    void SetModelStatus();



public: // Get function
    const Vector3& GetPosition()
    {
        return m_position;
    }

    const Quaternion& GetRotation()
    {
        return m_rotation;
    }

    const bool GetFlagAttackAnimation()
    {
        return m_flagAttackAnimation;
    }

    Bone& GetBone(const wchar_t boneName)
    {
        return m_modelCharacter->GetBone(boneName);
    }

    Skeleton& GetSkeleton()
    {
        return m_modelCharacter->GetSkeleton();
    }



public: // Set function
    void SetAttackAnimation(const int animation)
    {
        m_flagAttackAnimation = true;

        m_modelCharacter->PlayAnimation(idle);
        m_modelCharacter->PlayAnimation(animation); //�A�j���[�V�����̍Đ�
    }


    void SetTest(Hitbox& hitbox)
    {
        m_modelCharacter->SetTest(hitbox);
    }


public: //enum
    enum AnimationEnum
    {
        idle,
        jub,
        uppercut,
        hook,
        bodyBlow,
        straight,
        AnimationMax
    };


private: // constant


private: // data member
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // �v���C���[�L�����N�^�[�̃��f��

    ////////////////////////////////////////////////////////////
    // �L�����N�^�[�̃X�e�[�^�X
    ////////////////////////////////////////////////////////////

    Vector3 m_position = g_vec3Zero; // �ʒu
    Quaternion m_rotation = g_quatIdentity; // ��]
    float m_rotY = 0.0f; // ��]��
    Vector3 m_scale = g_vec3One; // �g��

    AnimationClip m_animationPlayer[AnimationMax]; // �A�j���[�V����

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // ����\��
    bool m_flagAttackAnimation = false; // �U�����̃A�j���[�V���������Ă��邩
};