////////////////////////////////////////////////////////////
// 使用しない
////////////////////////////////////////////////////////////
#pragma once

class Player;
class Actor;
struct StAttackData;

////////////////////////////////////////////////////////////
// 1.攻撃中かの確認
// 2.攻撃ターゲット部位の確認
// 3.攻撃ターゲットに向けて手のボーンの位置を調整する処理
// 3.1.とりあえず、横方向の調整はせずに、縦方向だけの調整をする。
////////////////////////////////////////////////////////////

/**
 * @brief 攻撃モーション時のボーンの移動処理
*/
class AttackMoveBone : public IGameObject
{
public:
    AttackMoveBone();
    ~AttackMoveBone();
    bool Start() override final;
    void Update() override final;


    void Init(Player& otherPlayer, Actor& actor, StAttackData& stAttackData);


private:
    const int CheckTargetBodyParts();


    void MoveBoneToAttackTheHead();

    void MoveBoneToAttackTheBody();



private: // enum
    /**
     * @brief どの部位を狙って攻撃しているか
    */
    enum EnTargetBodyParts
    {
        enHead,                 // 頭
        enBody,                 // 胴体
        enMaxTargetBodyParts
    };



private: // data member
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////



    //////////////////////////////
    // 他のクラスから持ってきたインスタンス
    //////////////////////////////

    Player* m_getOhterPlayer = nullptr;
    Actor* m_getActor = nullptr;
    StAttackData* m_getStAttackData = nullptr;
};