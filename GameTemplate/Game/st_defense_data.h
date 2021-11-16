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
     * @brief �h��l�̉�
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
    bool m_flagDefense = false; // �h�䒆��
    int m_defenseValue = m_INIT_DEFENSE_VALUE; // �h��l
    bool m_flagGuardBreak = false; // �K�[�h�u���C�N����

    int m_countGrardBreakTimer = 0; // �K�[�h�u���C�N���̃^�C�}�[
    static const int m_MAX_COUNT_GRARD_BRAEK_TIMER = 300; // �K�[�h�u���C�N�ŃX�^�����Ă��鎞��

    int m_defenseValueRecoveryTime = 0;
    static const int m_MAX_DEFENSE_VALUE_RECOVERY_TIME = 5;


};