#pragma once
#include "ghost_object.h"

class Player;
class GameData;

class AttackJudgment : public IGameObject
{
public:
    AttackJudgment();
    ~AttackJudgment();
    bool Start() override final;
    void Update() override final;

    /**
     * @brief 初期化（呼ばないと動かせないので必ず呼ぶこと）
     * @param playerNum プレイヤー番号
    */
    void Init(const int playerNum);


public:
    /**
     * @brief 攻撃判定が相手プレイヤーの当たり判定に当たったかの確認
     * @return 当たったか
    */
    const bool CheckHit();



public:
    /**
     * @brief 攻撃判定エリアの作成
     * @param pos 位置
     * @param rot 回転
    */
    void Create(const Vector3& pos, const Quaternion& rot, const Vector3& size);

    /**
     * @brief 攻撃判定エリアの削除
    */
    void Release()
    {
        if (false == m_flagInit) {
            return;
        }

        m_ghostBox->Release();
    }


private: // data menber
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    GhostObject* m_ghostBox = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////

    Player* m_findPlayer = nullptr;
    GameData* m_findGameData = nullptr;

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagInit = false; // 初期化したか
};