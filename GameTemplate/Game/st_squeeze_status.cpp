#include "stdafx.h"
#include "st_squeeze_status.h"

void StSqueezeStatus::StartSqueeze()
{
    m_flagSqueeze = true;
    m_squeezeTime = 0;
}

void StSqueezeStatus::SqueezeUpdate()
{
    if (false == m_flagSqueeze) {
        return;
    }

    ++m_squeezeTime;

    if (m_squeezeTimeLimit <= m_squeezeTime) {
        m_squeezeTime = 0;
        m_flagSqueeze = false;
    }
}