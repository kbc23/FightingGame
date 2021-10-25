#include "stdafx.h"
#include "st_dash_status.h"

void StDashStatus::StartDash()
{
    // ダッシュ中か残り回数が０のときは処理をしない
    if (true == m_flagDash || 0 >= m_remainingNumberOfTimes) {
        return;
    }

    --m_remainingNumberOfTimes; // 残り回数を１減少

    m_flagDash = true;
}

void StDashStatus::DashUpdate()
{
    DashRecoveryTime();

    if (false == m_flagDash) {
        return;
    }

    ++m_countDash;

    if (m_MAX_COUNT_DASH <= m_countDash) {
        m_flagDash = false;
        m_countDash = 0;
    }
}

void StDashStatus::DashRecoveryTime()
{
    // 残り回数が最大なら処理をしない
    if (m_MAX_REMAINING_NUMBER_OF_TIMES <= m_remainingNumberOfTimes) {
        return;
    }

    ++m_countRecoveryTime;

    if (m_MAX_RECOVERY_TIME <= m_countRecoveryTime) {
        ++m_remainingNumberOfTimes; // 残り回数を１増加
        m_countRecoveryTime = 0;
    }
}