#include "stdafx.h"
#include "st_player_status.h"

void StPlayerStatus::StatusUpdate()
{
    // 攻撃関連のUpdate
    m_attackData.Update();

    // ダッシュ関連のUpdate
    m_dashStatus.DashUpdate();

    // ノックバック関連のUpdate
    m_squeezeStatus.SqueezeUpdate();

    // ダウン関連のUpdate
    m_downStatus.DownUpdate();

    // ガード関連のUpdate
    m_defenceData.Update();
}

const bool StPlayerStatus::NotController()
{
    // 攻撃時、処理をしない
    if (true == m_attackData.GetFlagAttackNow()) {
        return true;
    }

    // ノックバック時、処理をしない
    if (true == m_squeezeStatus.GetFlagSqueeze()) {
        return true;
    }

    // ダウン時、処理をしない
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