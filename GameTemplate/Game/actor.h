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
        float initRotAngle,
        Actor* pOtherActor
    );

    /**
     * @brief 
     * @param filePath ���f���̃t�@�C���p�X
     * @param playerNum �v���C���[�ԍ�
     * @param initPos �����ʒu
     * @param initRot ��������
    */
    void DebugInit(const char* filePath, const int playerNum, const Vector3& initPos, const float initRot);

    /**
     * @brief ���������ɃX�e�[�^�X���X�V����
     * @param addMoveAmount �X�V����ړ���
     * @param addRotAngle �X�V�����]��
    */
    void AddStatus(Vector3& addMoveAmount, const float addRotAngle);



private:
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

    RigidBody* GetRigidBody()
    {
        return m_charaCon.GetRigidBody();
    }




private: // constant


private: // data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // �v���C���[�L�����N�^�[�̃��f��
    Actor* m_otherActor = nullptr; // �ΐ푊��
    CharacterController m_charaCon; // �L�����R��


    ////////////////////////////////////////////////////////////
    // �L�����N�^�[�̃X�e�[�^�X
    ////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f }; // �ʒu
    Quaternion m_rotation = g_quatIdentity; // ��]
    float m_rotY = 0.0f; // ��]��
    Vector3 m_scale = g_vec3One; // �g��




    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // ����\��

};