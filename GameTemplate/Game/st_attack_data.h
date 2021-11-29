#pragma once

/**
 * @brief 攻撃情報に関する構造体
*/
struct StAttackData
{
public: // function
    /**
     * @brief 攻撃情報をセット
     * @param attackType 攻撃タイプ
     * @return 情報をセットしたか
    */
    const bool SetAttackData(const int attackType);

    /**
     * @brief 毎フレームの処理
    */
    void Update();


private:
    /**
     * @brief 攻撃フレームの更新
     * @return 攻撃が終わったか
    */
    const bool AttackFrameUpdate();

    /**
     * @brief 攻撃の終了処理
    */
    void AttackFinish();


public: // get function
    /**
     * @brief 現在攻撃中かを取得
     * @return 現在攻撃中か
    */
    const bool GetFlagAttackNow() const
    {
        return m_flagAttackNow;
    }

    /**
     * @brief 攻撃力を取得
     * @return 攻撃力
    */
    const int GetPower() const
    {
        return m_power;
    }

    /**
     * @brief 攻撃がすでに当たったかを取得
     * @return 攻撃がすでに当たったか
    */
    const bool GetFlagAlreadyAttacked() const
    {
        return m_flagAlreadyAttacked;
    }

    /**
     * @brief 攻撃の種類を取得
     * @return 攻撃の種類
    */
    const int GetAttackType() const
    {
        return m_attackType;
    }


public: // Set function
    /**
     * @brief 攻撃がすでに当たったかをセット
     * @param flag 攻撃がすでに当たったか
    */
    void SetFlagAlreadyAttacked(const bool flag)
    {
        m_flagAlreadyAttacked = flag;
    }

    /**
     * @brief 攻撃をしていない状態にセット
    */
    void SetFalseFlagAttackNow()
    {
        m_flagAttackNow = false;
    }


public: // enum
    /**
     * @brief 攻撃の種類
    */
    enum EnAttackType
    {
        enNotAttacking, // 攻撃していない
        enJub,          // ジャブ
        enHook,         // フック
        enBodyBlow,     // ボディブロー
        enStraight,     // ストレート
        enUppercut,     // アッパーカット
    };

    /**
     * @brief 攻撃による影響
    */
    enum EnImpactType
    {
        enNotImpact,    // なにもない
        enSqueeze,      // のけぞり
        enDown          // ダウン
    };


private: // data member
    ////////////////////////////////////////////////////////////
    // ステータス
    ////////////////////////////////////////////////////////////

    int m_power = 0; // 攻撃力
    int m_attackType = EnAttackType::enNotAttacking; // 攻撃の種類
    //int m_impactType = EnImpactType::enNotImpact; // 攻撃による影響

    ////////////////////////////////////////////////////////////
    // 攻撃時間
    ////////////////////////////////////////////////////////////

    int m_attackTime = 0; // 攻撃時間
    int m_attackTimeLimit = 0; // 攻撃時間の上限

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagAlreadyAttacked = false; // 攻撃がもう当たっているか
    bool m_flagAttackNow = false; // 現在攻撃中か
};