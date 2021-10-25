#include "stdafx.h"
#include "st_dash_status.h"

void StDashStatus::StartDash()
{
    // �_�b�V�������c��񐔂��O�̂Ƃ��͏��������Ȃ�
    if (true == m_flagDash || 0 >= m_remainingNumberOfTimes) {
        return;
    }

    --m_remainingNumberOfTimes; // �c��񐔂��P����

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
    // �c��񐔂��ő�Ȃ珈�������Ȃ�
    if (m_MAX_REMAINING_NUMBER_OF_TIMES <= m_remainingNumberOfTimes) {
        return;
    }

    ++m_countRecoveryTime;

    if (m_MAX_RECOVERY_TIME <= m_countRecoveryTime) {
        ++m_remainingNumberOfTimes; // �c��񐔂��P����
        m_countRecoveryTime = 0;
    }
}