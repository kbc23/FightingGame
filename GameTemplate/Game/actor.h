#pragma once
#include "model_render.h"
#include "read_CSV_file_character_data.h"

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
    */
    void Init(
        const char* filePath,
        const Vector3& initPos,
        float initRotAngle
    );

    /**
     * @brief 
     * @param filePath ���f���̃t�@�C���p�X
     * @param initPos �����ʒu
     * @param initRot ��������
    */
    void DebugInit(const char* filePath, const Vector3& initPos, const float initRot);

    /**
     * @brief ���������ɃX�e�[�^�X���X�V����
     * @param addMoveAmount �X�V����ړ���
     * @param addRotAngle �X�V�����]��
    */
    void AddStatus(Vector3& addMoveAmount, const Vector2& swayMove);

    /**
     * @brief �L�����N�^�[���ړ������Ɍ�������
     * @param addMoveAmount �X�V����ړ���
    */
    void Turn(Vector3& addMoveAmount);


private:
    /**
     * @brief �U���֘A�̃A�j���[�V�����̏���
    */
    void AttackAnimation();

    /**
     * @brief ���f���ɃX�e�[�^�X����n��
    */
    void SetModelStatus();

    /**
     * @brief ���Ⴊ�݂̖��t���[���̏���
     * @param swayMoveY �X�E�F�[��Y���̈ړ���
    */
    void CrouchingUpdate(const float swayMoveY);

    /**
     * @brief ���Ⴊ�݂̊J�n
    */
    void CrouchingStart();

    /**
     * @brief ���Ⴊ�ݒ��̂��ꂱ��i���t���[���j
    */
    void Crouching();

    /**
     * @brief ���Ⴊ�݋N���オ��
    */
    void CrouchingEnd();


public: // Get function
    /**
     * @brief �ʒu���擾
     * @return �ʒu
    */
    const Vector3& GetPosition() const
    {
        return m_position;
    }

    /**
     * @brief ��]���擾
     * @return ��]
    */
    const Quaternion& GetRotation() const
    {
        return m_rotation;
    }

    /**
     * @brief �U���A�j���[�V�����������擾
     * @return �U���A�j���[�V��������
    */
    const bool GetFlagAttackAnimation() const
    {
        return m_flagAttackAnimation;
    }

    /**
     * @brief �w�肵�����O�̃{�[�����擾
     * @param boneName �{�[���̖��O
     * @return �w�肵�����O�̃{�[��
    */
    Bone& GetBone(const wchar_t boneName) const
    {
        return m_modelCharacter->GetBone(boneName);
    }

    /**
     * @brief �X�P���g�����擾
     * @return �X�P���g��
    */
    Skeleton& GetSkeleton() const
    {
        return m_modelCharacter->GetSkeleton();
    }

    Matrix& GetModelWorldMatrix()
    {
        m_modelCharacter->GetModelWorldMatrix();
    }


public: // Set function
    /**
     * @brief �w�肵���U���A�j���[�V�������Đ�
     * @param animation �U���A�j���[�V����
    */
    void SetAttackAnimation(const int animation)
    {
        m_flagAttackAnimation = true;

        // �����A�j���[�V�������w�肵���ہA�ŏ�����Đ������悤�ɒʏ펞�A�j���[�V����������
        m_modelCharacter->PlayAnimation(enIdle);
        //�A�j���[�V�����̍Đ�
        m_modelCharacter->PlayAnimation(animation);
    }

    /**
     * @brief ModelRender�N���X��Hitbox�N���X�̈ʒu���Ȃǂ̍X�V���������̂œn������
     * �i�֐��̖��O���ǂ��ɂ�����j
     * @param hitbox Hitbox�N���X�̃C���X�^���X
    */
    void SetTest(Hitbox& hitbox)
    {
        m_modelCharacter->SetTest(hitbox);
    }


public: //enum
    /**
     * @brief �A�j���[�V�����N���b�v
    */
    enum AnimationEnum
    {
        enIdle,
        enJub,
        enUppercut,
        enHook,
        enBodyBlow,
        enStraight,
        enCrouchingStart,
        enCrouching,
        enCrouchingEnd,
        AnimationMax
    };

    enum class EnCrouchingStatus
    {
        enNotCrouching,
        enStart,
        enCrouching,
        enEnd,
        MaxCrouchingStatus
    };

    EnCrouchingStatus m_crouchingStatus = EnCrouchingStatus::enNotCrouching;


private: // data member
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // �v���C���[�L�����N�^�[�̃��f��
    ReadCSVFileCharacterData m_readCSVFileCharacterData;

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