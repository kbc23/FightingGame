#pragma once


struct StDefenseData
{
public:
    void Update();

    void DecreaseDefenseValue(const int decreaseDefenseValue);


    void CountGuardBreakTime();


public: // Get function
    const bool GetFlagDefense()
    {
        return m_flagDefense;
    }
    
    const int GetDefenseValue()
    {
        return m_defenseValue;
    }

    const bool GetFlagGuardBreak()
    {
        return m_flagGuardBreak;
    }




public: // Set function
    void SetFlagDefense(const bool defense)
    {
        m_flagDefense = defense;
    }

    /**
     * @brief 防御値の回復
    */
    void AddDefenseValue()
    {
        if (m_INIT_DEFENSE_VALUE <= m_defenseValue) {
            return;
        }

        ++m_defenseValueRecoveryTime;

        if (m_MAX_DEFENSE_VALUE_RECOVERY_TIME <= m_defenseValueRecoveryTime) {
            ++m_defenseValue;
            m_defenseValueRecoveryTime = 0;

            if (m_INIT_DEFENSE_VALUE <= m_defenseValue) {
                m_defenseValue = m_INIT_DEFENSE_VALUE;
            }
        }
    }



public: // constant
    static const int m_INIT_DEFENSE_VALUE = 100;



private: // data member
    bool m_flagDefense = false; // 防御中か
    int m_defenseValue = m_INIT_DEFENSE_VALUE; // 防御値
    bool m_flagGuardBreak = false; // ガードブレイク中か

    int m_countGrardBreakTimer = 0; // ガードブレイク中のタイマー
    static const int m_MAX_COUNT_GRARD_BRAEK_TIMER = 300; // ガードブレイクでスタンしている時間

    int m_defenseValueRecoveryTime = 0;
    static const int m_MAX_DEFENSE_VALUE_RECOVERY_TIME = 5;


};