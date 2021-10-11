#pragma once

class Actor;
class PlayerCamera;

class Player : public IGameObject
{
public:
    Player();
    ~Player();
    bool Start() override;
    void Update() override;

    /**
     * @brief 初期化
     * @param gamePad キャラクターの移動に使用するゲームパッド
     * @param initPos
     * @param initRotAngle
     * @param pOtherActor
    */
    void Init(
        GamePad& gamePad,
        const Vector3& initPos,
        float initRotAngle,
        Actor* pOtherActor
    );

    void DebugInit(const char* filePath, const int playerNum, const Vector3& initPos);


private:
    /**
     * @brief プレイヤーの入力を受ける
    */
    void Controller();



private: // constant
    const int m_MAX_HP = 1000; // プレイヤーの体力の最大値


private: // data menber
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    Actor* m_actor = nullptr; // キャラクター
    GamePad* m_gamePad = nullptr;
    PlayerCamera* m_playerCamera = nullptr;

    ////////////////////////////////////////////////////////////
    // プレイヤーのステータス
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // プレイヤーの体力

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か


};