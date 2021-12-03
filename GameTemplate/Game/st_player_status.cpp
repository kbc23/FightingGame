#include "stdafx.h"
#include "st_player_status.h"

void StPlayerStatus::StatusUpdate()
{
    // �U���֘A��Update
    m_attackData.Update();

    // �_�b�V���֘A��Update
    m_dashStatus.DashUpdate();

    // �m�b�N�o�b�N�֘A��Update
    m_squeezeStatus.SqueezeUpdate();

    // �_�E���֘A��Update
    m_downStatus.DownUpdate();

    // �K�[�h�֘A��Update
    m_defenceData.Update();
}

const bool StPlayerStatus::NotController()
{
    // �U�����A���������Ȃ�
    if (true == m_attackData.GetFlagAttackNow()) {
        return true;
    }

    // �m�b�N�o�b�N���A���������Ȃ�
    if (true == m_squeezeStatus.GetFlagSqueeze()) {
        return true;
    }

    // �_�E�����A���������Ȃ�
    if (true == m_downStatus.GetFlagDown()) {
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