#include "stdafx.h"
#include "st_down_status.h"

void StDownStatus::StartDown()
{
    m_flagDown = true;
}

void StDownStatus::DownUpdate()
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