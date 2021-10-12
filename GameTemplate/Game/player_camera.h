#pragma once

class PlayerCamera : public IGameObject
{
public:
    PlayerCamera();
    ~PlayerCamera();
    bool Start() override;
    void Update() override;




public: // Set function
    void SetPlayerPosition(const Vector3& playerPos)
    {
        m_playerPosition = playerPos;
    }

    void SetEnemyPosition(const Vector3& enemyPos)
    {
        m_enemyPosition = enemyPos;
    }


private: // data menber
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////
    // ステータス
    ////////////////////////////////////////////////////////////
    Vector3 m_position = { 0.0f,0.0f,0.0f }; // 位置
    Vector3 m_target = { 0.0f,0.0f,0.0f }; // 注視点
    Vector3 m_playerPosition = { 0.0f,0.0f,0.0f }; // プレイヤーの位置
    Vector3 m_enemyPosition = { 0.0f,0.0f,0.0f }; // 相手プレイヤーの位置

};