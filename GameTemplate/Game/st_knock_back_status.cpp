#include "stdafx.h"
#include "st_knock_back_status.h"

void StKnockBackStatus::StartKnockBack()
{
    m_flagKnockBack = true;
}

void StKnockBackStatus::KnockBackUpdate()
{
    if (false == m_flagKnockBack) {
        return;
    }

    ++m_knockBackTime;

    if (m_knockBackTimeLimit <= m_knockBackTime) {
        m_knockBackTime = 0;
        m_flagKnockBack = false;
    }
}