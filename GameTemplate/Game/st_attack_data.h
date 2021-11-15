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


    void UpdateContinuousAttack();

    /**
     * @brief 攻撃を始めるまでのディレイの処理
    */
    const bool DelayAttack();

    /**
     * @brief 攻撃情報をリセット
    */
    void ResetAttackData();

    /**
     * @brief 攻撃が当たったときの攻撃データの確認
    */
    void HitCheckAttackData();



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

    const int GetImpactType()
    {
        return m_impactType;
    }

    const int GetAttackType()
    {
        return m_attackType;
    }

    const bool GetFlagNextAttackPossible()
    {
        return m_flagNextAttackPossible;
    }


public: // Set function
    void SetFlagAlreadyAttacked(const bool flag)
    {
        m_flagAlreadyAttacked = flag;
    }

    void SetFlagNextAttackPossible(const bool flag)
    {
        m_flagNextAttackPossible = flag;
    }


public: // enum
    /**
     * @brief 攻撃の種類
    */
    enum EnAttackType
    {
        enNotAttacking, // 攻撃していない
        enJub, // ジャブ
        enHook, // フック
        enBodyBlow, // ボディブロー
        enStraight, // ストレート
        enUppercut, // アッパーカット
    };

    /**
     * @brief 攻撃による影響
    */
    enum EnImpactType
    {
        enNotImpact, // なにもない
        enSqueeze, // のけぞり
        enDown // ダウン
    };


private: // data member
    ////////////////////////////////////////////////////////////
    // ステータス
    ////////////////////////////////////////////////////////////

    int m_power = 0; // 攻撃力
    int m_attackType = EnAttackType::enNotAttacking; // 攻撃の種類
    int m_impactType = EnImpactType::enNotImpact; // 攻撃による影響
    // 攻撃の情報がリセットされるまで、次に技キャンセルをして使用できない攻撃
    int m_nextNGAttackType = EnAttackType::enNotAttacking;
    // 攻撃の情報がリセットされるまで、次に唯一技キャンセルをして使用できる攻撃
    int m_nextOnlyOKAttackType = EnAttackType::enNotAttacking;
    // ※処理の優先度 ([m_nextOnlyOKAttackType] > [m_nextNGAttackType])

    ////////////////////////////////////////////////////////////
    // 攻撃時間
    ////////////////////////////////////////////////////////////

    int m_attackTime = 0; // 攻撃時間
    int m_attackTimeLimit = 0; // 攻撃時間の上限
    
    ////////////////////////////////////////////////////////////
    // 攻撃のディレイ
    ////////////////////////////////////////////////////////////

    int m_delayTime = 0; // 攻撃までのディレイ
    int m_delayTimeLimit = 0; // 攻撃までのディレイの上限
    bool m_flagFinishDelay = true; // 攻撃開始までのディレイが終わったか（ディレイがない場合[true]に設定）
    
    ////////////////////////////////////////////////////////////
    // 攻撃範囲
    ////////////////////////////////////////////////////////////

    Vector3 m_range = Vector3::Zero; //攻撃範囲
    float m_positionUpY = 0.0f;
    float m_positionUpZ = 0.0f;

    ////////////////////////////////////////////////////////////
    // 連続攻撃
    ////////////////////////////////////////////////////////////

    int m_countContinuousAttack = 0; // 連続攻撃の回数
    //int m_maxCountContinuousAttack = 10; // 連続攻撃の最大数
    static const int m_MAX_COUNT_CONTINUOUS_ATTACK = 4; // 連続攻撃の最大数 - 1
    int m_continuousAttackGraceTime = 0; // 連続攻撃の猶予時間
    //int m_continuousAttackGraceTimeLimit = 0; // 連続攻撃の猶予時間の上限
    static const int m_CONTINUOUS_ATTACK_GRACE_TIME_LIMIT = 90; // 連続攻撃の猶予時間の上限
    bool m_flagContinuousAttack = false; // 連続攻撃中か

    ////////////////////////////////////////////////////////////
    // 連続攻撃中のジャブに関係するデータ
    ////////////////////////////////////////////////////////////

    int m_continueJubAttack = 0; // 連続攻撃で当たったジャブの数
    static const int m_MAX_CONTINUE_JUB_ATTACK = 2; // 連続攻撃で使用できるジャブの最大数 - 1

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagStartAttack = false; // 攻撃を開始したか
    bool m_flagAlreadyAttacked = false; // 攻撃がもう当たっているか
    bool m_flagAttackNow = false; // 現在攻撃中か
    bool m_flagCreateAttackRange = false; // 攻撃判定を作成したか
    bool m_flagNextAttackPossible = true; // 次の攻撃ができるか
};