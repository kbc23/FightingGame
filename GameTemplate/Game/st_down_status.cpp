#include "stdafx.h"
#include "st_down_status.h"

void StDownStatus::StartDown()
{
    m_flagDown = true;
    m_flagInvincible = true;
}

void StDownStatus::DownUpdate()
{
    DownCount();
    InvincibleCount();
}

void StDownStatus::DownCount()
{
    if (false == m_flagDown) {
        return;
    }

    ++m_downTime;

    if (m_downTimeLimit <= m_downTime) {
        m_downTime = 0;
        m_flagDown = false;
    }
}

void StDownStatus::InvincibleCount()
{
    if (false == m_flagInvincible) {
        return;
    }

    ++m_invincibleTime;

    if (m_invincibleTimeLimit <= m_invincibleTime) {
        m_invincibleTime = 0;
        m_flagInvincible = false;
    }
}