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
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////
    // ステータス
    ////////////////////////////////////////////////////////////

    Vector3 m_cameraPosition = g_vec3Zero; // 位置
    Vector3 m_cameraTarget = g_vec3Zero; // 注視点
    Vector3 m_playerPosition = g_vec3Zero; // プレイヤーの位置
    Vector3 m_enemyPosition = g_vec3Zero; // 相手プレイヤーの位置

    Quaternion m_playerRotation = g_quatIdentity;

    Vector3 m_otherCameraPosition = g_vec3Zero; // 位置
    Vector3 m_otherCameraTarget = g_vec3Zero; // 注視点


    ////////////////////////////////////////////////////////////
    // 仮置き
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1;
    int m_otherPlayerNum = -1;


};