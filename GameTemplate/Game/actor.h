#pragma once
#include "model_render.h"

////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////

class Actor : public IGameObject
{
public:
    Actor();
    ~Actor();
    bool Start() override;
    void Update() override;

    void Init(
        GamePad& gamePad,
        const char* filePath,
        const Vector3& initPos,
        float initRotAngle,
        Actor* pOtherActor
    );

    void DebugInit(const char* filePath, const int playerNum, const Vector3& initPos);



private:
    /**
     * @brief �v���C���[�̓��͂��󂯂�
    */
    void Controller();

    /**
     * @brief �L�����N�^�[���f���ɕK�v�ȃX�e�[�^�X����n��
    */
    void SetModelStatus();




private: // constant
    const int m_MAX_HP = 1000; // �v���C���[�̗̑͂̍ő�l


private: // data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // �v���C���[�L�����N�^�[�̃��f��
    GamePad* m_gamePad = nullptr;
    Actor* m_otherActor = nullptr; // �ΐ푊��

    ////////////////////////////////////////////////////////////
    // �L�����N�^�[�̃X�e�[�^�X
    ////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity; // ��]
    float m_rotY = 0.0f;
    Vector3 m_scale = g_vec3One; // �g��


    int m_hp = m_MAX_HP; // �v���C���[�̗̑�

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // ����\��

};