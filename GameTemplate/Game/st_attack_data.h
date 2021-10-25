#pragma once

/**
 * @brief 攻撃情報に関する構造体
*/
struct StAttackData
{
public: // function
    void SetAttackData(const int attackType);

    /**
     * @brief 攻撃範囲の位置情報を作成
     * 位置がプレイヤーの前方のときに使用
     * @param playerPosition プレイヤーの位置
     * @param playerRotation プレイヤーの回転
     * @return 攻撃範囲の位置
    */
    const Vector3& CreateAttackPosition(const Vector3& playerPosition, const Quaternion& playerRotation);
    

    const bool UpdateFinish();

    /**
     * @brief 攻撃を始めるまでのディレイの処理
    */
    const bool DelayAttack();

    /**
     * @brief 攻撃情報をリセット
    */
    void ResetAttackData();



public: // Get function
    const Vector3& GetRange()
    {
        return m_range;
    }

    const bool GetFlagCreateAttackRange()
    {
        return m_flagCreateAttackRange;
    }

    const bool GetFlagAttackNow()
    {
        return m_flagAttackNow;
    }

    const int GetPower()
    {
        return m_power;
    }

    const bool GetFlagAlreadyAttacked()
    {
        return m_flagAlreadyAttacked;
    }


public: // Set function
    void SetFlagAlreadyAttacked(const bool flag)
    {
        m_flagAlreadyAttacked = flag;
    }


public:
    /**
     * @brief 攻撃の種類
    */
    enum EnAttackType
    {
        enNotAttacking, // 攻撃していない
        enNormal, // 通常攻撃
        enSub, // サブ攻撃
    };


private: // data member
    // status
    int m_power = 0; // 攻撃力
    int m_attackType = EnAttackType::enNotAttacking; // 攻撃の種類
    // attack time
    int m_attackTime = 0; // 攻撃時間
    int m_attackTimeLimit = 0; // 攻撃時間の上限
    // delay time
    int m_delayTime = 0; // 攻撃までのディレイ
    int m_delayTimeLimit = 0; // 攻撃までのディレイの上限
    bool m_flagFinishDelay = true; // 攻撃開始までのディレイが終わったか（ディレイがない場合[true]に設定）
    // attack range
    Vector3 m_range = Vector3::Zero; //攻撃範囲
    float m_positionUpY = 0.0f; // 上昇させる攻撃範囲のY座標の量
    // flag
    bool m_flagAlreadyAttacked = false; // 攻撃がもう当たっているか
    bool m_flagAttackNow = false; // 現在攻撃中か
    bool m_flagCreateAttackRange = false; // 攻撃判定を作成したか
};