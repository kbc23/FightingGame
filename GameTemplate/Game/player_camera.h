#pragma once

class PlayerCamera : public IGameObject
{
public:
    PlayerCamera();
    ~PlayerCamera();
    bool Start() override;
    void Update() override;


    void UpdatePlayerCamera();
    void UpdateOtherPlayerCamera();


public: // Get function
    const Vector3& GetPosition(const int playerNum)
    {
        if (playerNum == m_playerNum) {
            return m_cameraPosition;
        }
        else if (playerNum == m_otherPlayerNum) {
            return m_cameraPosition;
        }

        return { 0.0f,0.0f,0.0f };
    }



public: // Set function
    void SetPlayerPosition(const Vector3& playerPos)
    {
        m_playerPosition = playerPos;
    }

    void SetEnemyPosition(const Vector3& enemyPos)
    {
        m_enemyPosition = enemyPos;
    }

    void SetPlayerNum(const int playerNum, const int otherPlayerNum)
    {
        m_playerNum = playerNum;
        m_otherPlayerNum = otherPlayerNum;
    }


private: // data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////
    // �X�e�[�^�X
    ////////////////////////////////////////////////////////////

    Vector3 m_cameraPosition = { 0.0f,0.0f,0.0f }; // �ʒu
    Vector3 m_cameraTarget = { 0.0f,0.0f,0.0f }; // �����_
    Vector3 m_playerPosition = { 0.0f,0.0f,0.0f }; // �v���C���[�̈ʒu
    Vector3 m_enemyPosition = { 0.0f,0.0f,0.0f }; // ����v���C���[�̈ʒu

    Vector3 m_otherCameraPosition = { 0.0f,0.0f,0.0f }; // �ʒu
    Vector3 m_otherCameraTarget = { 0.0f,0.0f,0.0f }; // �����_


    ////////////////////////////////////////////////////////////
    // ���u��
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1;
    int m_otherPlayerNum = -1;


};