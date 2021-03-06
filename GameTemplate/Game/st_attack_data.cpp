#include "stdafx.h"
#include "st_attack_data.h"



namespace nsAttackData
{
    // ジャブ
    namespace nsJub
    {
        const int POWER = 20; // 攻撃力
        const int ATTACK_TIME_LIMIT = 16; // 攻撃時間
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // 攻撃による影響
    }
    
    // アッパーカット
    namespace nsUppercut
    {
        const int POWER = 16; // 攻撃力
        const int ATTACK_TIME_LIMIT = 15; // 攻撃時間
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // 攻撃による影響
    }

    // フック
    namespace nsHook
    {
        const int POWER = 13; // 攻撃力
        const int ATTACK_TIME_LIMIT = 15; // 攻撃時間
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // 攻撃による影響
    }

    // ボディブロー
    namespace nsBodyBlow
    {
        const int POWER = 25; // 攻撃力
        const int ATTACK_TIME_LIMIT = 21; // 攻撃時間
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // 攻撃による影響
    }

    // ストレート
    namespace nsStraight
    {
        const int POWER = 30; // 攻撃力
        const int ATTACK_TIME_LIMIT = 22; // 攻撃時間
        const int IMPACT_TYPE = StAttackData::EnImpactType::enDown; // 攻撃による影響
    }
}



const bool StAttackData::SetAttackData(const int attackType)
{
    // ジャブ
    if (EnAttackType::enJubHead_L == attackType) {
        m_power = nsAttackData::nsJub::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsJub::ATTACK_TIME_LIMIT; // 攻撃時間
        m_attackType = EnAttackType::enJubHead_L; // 攻撃の種類
        //m_impactType = nsAttackData::nsJub::IMPACT_TYPE; // 攻撃による影響
    }
    else if (EnAttackType::enJubHead_R == attackType) {
        m_power = nsAttackData::nsJub::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsJub::ATTACK_TIME_LIMIT; // 攻撃時間
        m_attackType = EnAttackType::enJubHead_R; // 攻撃の種類
        //m_impactType = nsAttackData::nsJub::IMPACT_TYPE; // 攻撃による影響
    }
    else if (EnAttackType::enJubBody_L == attackType) {
        m_power = nsAttackData::nsJub::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsJub::ATTACK_TIME_LIMIT; // 攻撃時間
        m_attackType = EnAttackType::enJubBody_L; // 攻撃の種類
        //m_impactType = nsAttackData::nsJub::IMPACT_TYPE; // 攻撃による影響
    }
    else if (EnAttackType::enJubBody_R == attackType) {
        m_power = nsAttackData::nsJub::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsJub::ATTACK_TIME_LIMIT; // 攻撃時間
        m_attackType = EnAttackType::enJubBody_R; // 攻撃の種類
        //m_impactType = nsAttackData::nsJub::IMPACT_TYPE; // 攻撃による影響
    }
    // アッパーカット
    else if (EnAttackType::enUppercut == attackType) {
        m_power = nsAttackData::nsUppercut::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsUppercut::ATTACK_TIME_LIMIT; // 攻撃時間
        m_attackType = EnAttackType::enUppercut; // 攻撃の種類
        //m_impactType = nsAttackData::nsUppercut::IMPACT_TYPE; // 攻撃による影響
    }
    // フック
    else if (EnAttackType::enHook == attackType) {
        m_power = nsAttackData::nsHook::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsHook::ATTACK_TIME_LIMIT; // 攻撃時間
        m_attackType = EnAttackType::enHook; // 攻撃の種類
        //m_impactType = nsAttackData::nsHook::IMPACT_TYPE; // 攻撃による影響
    }
    // ボディブロー
    else if (EnAttackType::enBodyBlow == attackType) {
        m_power = nsAttackData::nsBodyBlow::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsBodyBlow::ATTACK_TIME_LIMIT; // 攻撃時間
        m_attackType = EnAttackType::enBodyBlow; // 攻撃の種類
        //m_impactType = nsAttackData::nsBodyBlow::IMPACT_TYPE; // 攻撃による影響
    }
    // ストレート
    else if (EnAttackType::enStraight == attackType) {
        m_power = nsAttackData::nsStraight::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsStraight::ATTACK_TIME_LIMIT; // 攻撃時間
        m_attackType = EnAttackType::enStraight; // 攻撃の種類
        //m_impactType = nsAttackData::nsStraight::IMPACT_TYPE; // 攻撃による影響
    }

    m_flagAttackNow = true; // 現在攻撃中か

    return true;
}

void StAttackData::Update()
{
    if (false == m_flagAttackNow) {
        return;
    }

    // 攻撃している最中か？
    // 攻撃フレームを進める
    if (false == AttackFrameUpdate()) {
        // 攻撃が終わっていない
        return;
    }

    // 攻撃が終わった状態になったか？
    // 攻撃判定を削除
    AttackFinish();
}

const bool StAttackData::AttackFrameUpdate()
{
    ++m_attackTime;

    if (m_attackTimeLimit <= m_attackTime) {
        m_attackTime = 0;

        return true;
    }

    return false;
}

void StAttackData::AttackFinish()
{
    m_flagAttackNow = false;
    m_flagAlreadyAttacked = false;
}