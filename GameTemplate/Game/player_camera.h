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
            return m_otherCameraPosition;
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


private: // data member
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////
    // �X�e�[�^�X
    ////////////////////////////////////////////////////////////

    Vector3 m_cameraPosition = g_vec3Zero; // �ʒu
    Vector3 m_cameraTarget = g_vec3Zero; // �����_
    Vector3 m_playerPosition = g_vec3Zero; // �v���C���[�̈ʒu
    Vector3 m_enemyPosition = g_vec3Zero; // ����v���C���[�̈ʒu

    Quaternion m_playerRotation = g_quatIdentity;

    Vector3 m_otherCameraPosition = g_vec3Zero; // �ʒu
    Vector3 m_otherCameraTarget = g_vec3Zero; // �����_


    ////////////////////////////////////////////////////////////
    // ���u��
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1;
    int m_otherPlayerNum = -1;


};