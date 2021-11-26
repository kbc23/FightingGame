#pragma once
#include "ghost_object.h"

class Player;
class Actor;
class StAttackData;

/**
 * @brief 当たり判定の処理
 * ～攻撃判定もあるよ！～
*/
class Hitbox : public IGameObject
{
public:
    Hitbox();
    ~Hitbox();
    bool Start() override final;
    void Init(Player& otherPlayer, Actor& actor, StAttackData& attackData);
    void Update() override final;


    const bool UpdateCheckAttack();


private:
    void Create();


    void UpdateHitbox();



    /**
     * @brief 攻撃判定が相手プレイヤーの当たり判定に当たったかの確認
     * @return 当たったか
    */
    const bool CheckHit();

    void HitAttack();



private:
    /**
     * @brief 攻撃判定エリアの削除
    */
    void Release()
    {
        if (false == m_flagInit) {
            return;
        }

        for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
            m_ghostBox[bodyPartsNum]->Release();
        }
    }


public: // get function
    btGhostObject& GetGhostObject(const int bodyParts)
    {
        return *m_ghostBox[bodyParts]->GetGhostObject();
    }



private: // enum

    // モデルは左右反転している

    /**
     * @brief 身体の部位
    */
    const enum EnBodyParts
    {
        enHead,             // 頭
        enBody,             // 身体
        enButt,             // 尻
        // 左
        enLeftUpperArm,     // 上腕
        enLeftForearm,      // 前腕
        enLeftHand,         // 手
        enLeftThighs,       // 太もも
        enLeftLowerLeg,     // 下腿
        enLeftLegs,         // 足
        // 右
        enRightUpperArm,    // 上腕
        enRightForearm,     // 前腕
        enRightHand,        // 手
        enRightThighs,      // 太もも
        enRightLowerLeg,    // 下腿
        enRightLegs,        // 足
        enMaxBodyParts
    };


public:
    static const int m_MAX_EN_BODY_PARTS = EnBodyParts::enMaxBodyParts;




private: // data member
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    // ゴーストオブジェクト（当たり判定に使用するボックス）
    using GhostObjectPtr = std::unique_ptr<GhostObject>;
    std::vector<GhostObjectPtr> m_ghostBox;

    //////////////////////////////
    // その他
    //////////////////////////////

    Player* m_getOtherPlayer = nullptr;
    Actor* m_getActor = nullptr;
    StAttackData* m_getStAttackData = nullptr;

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagInit = false; // 初期化したか
};