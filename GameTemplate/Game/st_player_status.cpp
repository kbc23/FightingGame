#include "stdafx.h"
#include "st_player_status.h"

void StPlayerStatus::StatusUpdate()
{
    // �U���֘A��Update
    m_attackData.Update();

    // �_�b�V���֘A��Update
    m_dashStatus.DashUpdate();

    // �K�[�h�֘A��Update
    m_defenceData.Update();
}

const bool StPlayerStatus::NotController()
{
    // �U�����A���������Ȃ�
    if (true == m_attackData.GetFlagAttackNow()) {
        return true;
    }

    return false;
}

const bool StPlayerStatus::NotAttack()
{
    if (true == m_defenceData.GetFlagDefense()) {
        return true;
    }
    if (true == m_dashStatus.GetFlagDash()) {
        return true;
    }

    return false;
}