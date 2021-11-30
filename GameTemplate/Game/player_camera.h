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
    /**
     * @brief 指定したプレイヤーのカメラの位置を取得
     * @param playerNum プレイヤー番号
     * @return 指定したプレイヤーのカメラの位置
    */
    const Vector3 GetPosition(const int playerNum) const
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
    /**
     * @brief プレイヤーの位置をセット
     * @param playerPos プレイヤーの位置
    */
    void SetPlayerPosition(const Vector3& playerPos)
    {
        m_playerPosition = playerPos;
    }

    /**
     * @brief 敵の位置をセット
     * @param enemyPos 敵の位置
    */
    void SetEnemyPosition(const Vector3& enemyPos)
    {
        m_enemyPosition = enemyPos;
    }

    /**
     * @brief 自身と相手のプレイヤー番号をセット
     * @param playerNum 自身のプレイヤー番号
     * @param otherPlayerNum 相手のプレイヤー番号
    */
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

    Vector3 m_otherCameraPosition = g_vec3Zero; // 位置
    Vector3 m_otherCameraTarget = g_vec3Zero; // 注視点

    ////////////////////////////////////////////////////////////
    // 仮置き
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1; // 自身のプレイヤー番号
    int m_otherPlayerNum = -1; // 相手のプレイヤー番号
};