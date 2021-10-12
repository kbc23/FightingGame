#pragma once
#include "actor.h"

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
     * @param initPos 初期位置
     * @param initRotAngle 初期向き
     * @param pOtherPlayer 相手プレイヤー
    */
    void Init(
        GamePad& gamePad,
        const Vector3& initPos,
        float initRotAngle,
        Player* pOtherPlayer
    );

    /**
     * @brief Debugモード時の初期化
     * @param filePath モデルのファイルパス
     * @param playerNum プレイヤー番号
     * @param initPos 初期位置
     * @param initRot 初期向き
    */
    void DebugInit(const char* filePath, const int playerNum, const Vector3& initPos, const float initRot);


private:
    /**
     * @brief プレイヤーの入力を受ける
    */
    void Controller();

    /**
     * @brief プレイヤーの移動量を計算する
     * @return 移動量
    */
    const Vector3& Move();


public: // Get function
    const Vector3& GetPosition()
    {
        return m_actor->GetPosition();
    }


private: // constant
    const int m_MAX_HP = 1000; // プレイヤーの体力の最大値


private: // data menber
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    Actor* m_actor = nullptr; // キャラクター
    GamePad* m_gamePad = nullptr; // ゲームパッド
    Player* m_otherPlayer = nullptr; // 対戦相手


    ////////////////////////////////////////////////////////////
    // プレイヤーのステータス
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // プレイヤーの体力

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か


};