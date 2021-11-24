#pragma once
#include "ghost_object.h"

class Actor;

/**
 * @brief 当たり判定の処理
*/
class Hitbox : public IGameObject
{
public:
    Hitbox();
    ~Hitbox();
    bool Start() override final;
    void Init(Actor& actor);
    void Update() override final;


private:
    void Create();


    void UpdateHitbox();



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



private: // enum

    // モデルは左右反転している

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

    GhostObject* m_ghostBox[EnBodyParts::enMaxBodyParts] = { nullptr };

    //////////////////////////////
    // その他
    //////////////////////////////

    Actor* m_getActor = nullptr;

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagInit = false; // 初期化したか
};