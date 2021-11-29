#pragma once
#include  "constant.h"
#include "ghost_object.h"

class Player;
class Actor;
struct StAttackData;
struct StDefenseData;

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
    void Update() override final;

    /**
     * @brief 初期化
     * @param otherPlayer 相手プレイヤーのインスタンス
     * @param actor Actorクラスのインスタンス
     * @param attackData 攻撃情報のインスタンス
     * @param defenseData 防御情報のインスタンス
    */
    void Init(Player& otherPlayer, Actor& actor, StAttackData& attackData, StDefenseData& defenseData);

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

    /**
     * @brief 防御時に手が上腕に当たったかを確認
     * @param bodyParts 当たった部位
     * @return 手か上腕に当たっていたか
    */
    const bool CheckHitDefenseBodyParts(const int bodyParts);

    /**
     * @brief 攻撃が当たったときの処理
    */
    void HitAttack();


public: // get function
    /**
     * @brief 当たり判定のゴーストオブジェクトを取得
     * @param bodyParts 身体の部位
     * @return 指定した身体の部位の当たり判定のゴーストオブジェクト
    */
    btGhostObject& GetGhostObject(const int bodyParts) const
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
        // 最大値
        enMaxBodyParts
    };


public: // constant
    static const int m_MAX_BODY_PARTS = EnBodyParts::enMaxBodyParts; // 身体の部位の最大値（配列の要素数）


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
    // 他のクラスから持ってきたインスタンス
    //////////////////////////////

    Player* m_getOtherPlayer = nullptr; // 相手プレイヤー
    Actor* m_getActor = nullptr; // キャラクターのモデル関連
    StAttackData* m_getStAttackData = nullptr; // 攻撃情報
    StDefenseData* m_getStDefenseData = nullptr; // 防御情報

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagInit = false; // 初期化したか
};