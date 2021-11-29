#pragma once
#include  "constant.h"
#include "ghost_object.h"

class Player;
class Actor;
struct StAttackData;
struct StDefenseData;

/**
 * @brief 当たり判定の処理
 * 〜攻撃判定もあるよ！〜
*/
class Hitbox : public IGameObject
{
public:
    Hitbox();
    ~Hitbox();
    bool Start() override final;
    void Init(Player& otherPlayer, Actor& actor, StAttackData& attackData, StDefenseData& defenseData);
    void Update() override final;

    /**
     * @brief 攻撃が当たったかの毎フレームの処理
     * @return 
    */
    const bool UpdateCheckAttack();

    /**
     * @brief 当たり判定の情報の更新
    */
    void UpdateHitbox();


private:
    /**
     * @brief 当たり判定を作成
    */
    void Create();

    /**
     * @brief 攻撃判定が相手プレイヤーの当たり判定に当たったかの確認
     * @return 当たったか
    */
    const int CheckHit();


    const bool CheckHitDefenseBodyParts(const int bodyParts);


    /**
     * @brief 攻撃が当たったときの処理
    */
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
    /**
     * @brief 当たり判定のゴーストオブジェクトを取得
     * @param bodyParts 身体の部位
     * @return 指定した身体の部位の当たり判定のゴーストオブジェクト
    */
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
    static const int m_MAX_BODY_PARTS = EnBodyParts::enMaxBodyParts;






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
    StDefenseData* m_getStDefenseData = nullptr;

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagInit = false; // 初期化したか
};