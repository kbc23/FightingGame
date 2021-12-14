#pragma once
#include "st_attack_data.h"
#include "st_dash_status.h"
#include "st_squeeze_status.h"
#include "st_down_status.h"
#include "st_defense_data.h"  

/**
 * @brief プレイヤーのステータスを管理
*/
struct StPlayerStatus
{
public:
    /**
     * @brief ステータス関連のUpdate
    */
    void StatusUpdate();


    const bool NotController();

    const bool NotAttack();


public: // Check function
    const bool CheckNowDefence()
    {
        return m_defenceData.GetFlagDefense();
    }

    const bool CheckNowDash()
    {
        return m_dashStatus.GetFlagDash();
    }

    const bool CheckNowAttack()
    {
        return m_attackData.GetFlagAttackNow();
    }

    const bool CheckNowAlreadyAttacked()
    {
        return m_attackData.GetFlagAlreadyAttacked();
    }


public: // Get function
    const int GetAttackPower()
    {
        return m_attackData.GetPower();
    }

    const int GetNowAttackType()
    {
        return m_attackData.GetAttackType();
    }



public: // Set function
    void StartDefence()
    {
        m_defenceData.SetFlagDefense(true);
    }

    void EndDefence()
    {
        m_defenceData.SetFlagDefense(false);
    }

    void StartDash()
    {
        m_dashStatus.StartDash();
    }

    void SetAttackData(const int button)
    {
        if (enButtonRB1 == button) {
            m_attackData.SetAttackData(m_attackData.EnAttackType::enJubHead_L);
        }
        if (enButtonLB1 == button) {
            m_attackData.SetAttackData(m_attackData.EnAttackType::enJubHead_R);
        }
        if (enButtonRB2 == button) {
            m_attackData.SetAttackData(m_attackData.EnAttackType::enJubBody_L);
        }
        if (enButtonLB2 == button) {
            m_attackData.SetAttackData(m_attackData.EnAttackType::enJubBody_R);
        }
        //if (enButtonRB2 == button) {
        //    m_attackData.SetAttackData(m_attackData.EnAttackType::enBodyBlow);
        //}
    }

    void SetAlreadyAttacked()
    {
        m_attackData.SetFlagAlreadyAttacked(true);
    }
        




private: // data member
    StAttackData m_attackData; // 攻撃関連のデータ
    StDashStatus m_dashStatus; // ダッシュ関連のデータ
    StSqueezeStatus m_squeezeStatus; // のけぞり関連のデータ
    StDownStatus m_downStatus; // ダウン関連のデータ
    StDefenseData m_defenceData; // 防御関連のデータ
};