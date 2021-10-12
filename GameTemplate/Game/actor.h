#pragma once
#include "model_render.h"

////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////

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
     * @param gamePad �L�����N�^�[�̈ړ��Ɏg�p����Q�[���p�b�h
     * @param filePath 
     * @param initPos 
     * @param initRotAngle 
     * @param pOtherActor 
    */
    void Init(
        GamePad& gamePad,
        const char* filePath,
        const Vector3& initPos,
        float initRotAngle,
        Actor* pOtherActor
    );

    void DebugInit(const char* filePath, const int playerNum, const Vector3& initPos, const float initRot);

    void AddStatus(Vector3& addMoveAmount, const float addRotAngle);



private:
    /**
     * @brief �L�����N�^�[���f���ɕK�v�ȃX�e�[�^�X����n��
    */
    void SetModelStatus();

    void Move();



public: // Get function
    const Vector3& GetPosition()
    {
        return m_position;
    }

    const Quaternion& GetRotation()
    {
        return m_rotation;
    }




private: // constant


private: // data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // �v���C���[�L�����N�^�[�̃��f��
    //GamePad* m_gamePad = nullptr;
    Actor* m_otherActor = nullptr; // �ΐ푊��
    CharacterController m_charaCon;


    ////////////////////////////////////////////////////////////
    // �L�����N�^�[�̃X�e�[�^�X
    ////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity; // ��]
    float m_rotY = 0.0f;
    Vector3 m_scale = g_vec3One; // �g��




    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // ����\��

};