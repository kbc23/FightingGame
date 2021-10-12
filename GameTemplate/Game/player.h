#pragma once
#include "actor.h"

class PlayerCamera;

class Player : public IGameObject
{
public:
    Player();
    ~Player();
    bool Start() override;
    void Update() override;

    /**
     * @brief ������
     * @param gamePad �L�����N�^�[�̈ړ��Ɏg�p����Q�[���p�b�h
     * @param initPos
     * @param initRotAngle
     * @param pOtherActor
    */
    void Init(
        GamePad& gamePad,
        const Vector3& initPos,
        float initRotAngle,
        Player* pOtherPlayer
    );

    void DebugInit(const char* filePath, const int playerNum, const Vector3& initPos, const float initRot);


private:
    /**
     * @brief �v���C���[�̓��͂��󂯂�
    */
    void Controller();


    const Vector3& Move();


public: // Get function
    const Vector3& GetPosition()
    {
        return m_actor->GetPosition();
    }


private: // constant
    const int m_MAX_HP = 1000; // �v���C���[�̗̑͂̍ő�l


private: // data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    Actor* m_actor = nullptr; // �L�����N�^�[
    GamePad* m_gamePad = nullptr;
    PlayerCamera* m_findPlayerCamera = nullptr;
    Player* m_otherPlayer = nullptr; // �ΐ푊��


    ////////////////////////////////////////////////////////////
    // �v���C���[�̃X�e�[�^�X
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // �v���C���[�̗̑�

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // ����\��


};