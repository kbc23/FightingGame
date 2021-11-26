#include "stdafx.h"
#include "st_attack_data.h"



namespace nsAttackData
{
    // ƒWƒƒƒu
    namespace nsJub
    {
        const int POWER = 8; // UŒ‚—Í
        const int ATTACK_TIME_LIMIT = 16; // UŒ‚ŠÔ
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // UŒ‚‚É‚æ‚é‰e‹¿
    }
    
    // ƒAƒbƒp[ƒJƒbƒg
    namespace nsUppercut
    {
        const int POWER = 16; // UŒ‚—Í
        const int ATTACK_TIME_LIMIT = 15; // UŒ‚ŠÔ
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // UŒ‚‚É‚æ‚é‰e‹¿
    }

    // ƒtƒbƒN
    namespace nsHook
    {
        const int POWER = 13; // UŒ‚—Í
        const int ATTACK_TIME_LIMIT = 15; // UŒ‚ŠÔ
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // UŒ‚‚É‚æ‚é‰e‹¿
    }

    // ƒ{ƒfƒBƒuƒ[
    namespace nsBodyBlow
    {
        const int POWER = 25; // UŒ‚—Í
        const int ATTACK_TIME_LIMIT = 21; // UŒ‚ŠÔ
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // UŒ‚‚É‚æ‚é‰e‹¿
    }

    // ƒXƒgƒŒ[ƒg
    namespace nsStraight
    {
        const int POWER = 30; // UŒ‚—Í
        const int ATTACK_TIME_LIMIT = 22; // UŒ‚ŠÔ
        const int IMPACT_TYPE = StAttackData::EnImpactType::enDown; // UŒ‚‚É‚æ‚é‰e‹¿
    }
}



const bool StAttackData::SetAttackData(const int attackType)
{
    // ƒWƒƒƒu
    if (EnAttackType::enJub == attackType) {
        m_power = nsAttackData::nsJub::POWER; // UŒ‚—Í
        m_attackTimeLimit = nsAttackData::nsJub::ATTACK_TIME_LIMIT; // UŒ‚ŠÔ
        m_attackType = EnAttackType::enJub; // UŒ‚‚Ìí—Ş
        //m_impactType = nsAttackData::nsJub::IMPACT_TYPE; // UŒ‚‚É‚æ‚é‰e‹¿
    }
    // ƒAƒbƒp[ƒJƒbƒg
    else if (EnAttackType::enUppercut == attackType) {
        m_power = nsAttackData::nsUppercut::POWER; // UŒ‚—Í
        m_attackTimeLimit = nsAttackData::nsUppercut::ATTACK_TIME_LIMIT; // UŒ‚ŠÔ
        m_attackType = EnAttackType::enUppercut; // UŒ‚‚Ìí—Ş
        //m_impactType = nsAttackData::nsUppercut::IMPACT_TYPE; // UŒ‚‚É‚æ‚é‰e‹¿
    }
    // ƒtƒbƒN
    else if (EnAttackType::enHook == attackType) {
        m_power = nsAttackData::nsHook::POWER; // UŒ‚—Í
        m_attackTimeLimit = nsAttackData::nsHook::ATTACK_TIME_LIMIT; // UŒ‚ŠÔ
        m_attackType = EnAttackType::enHook; // UŒ‚‚Ìí—Ş
        //m_impactType = nsAttackData::nsHook::IMPACT_TYPE; // UŒ‚‚É‚æ‚é‰e‹¿
    }
    // ƒ{ƒfƒBƒuƒ[
    else if (EnAttackType::enBodyBlow == attackType) {
        m_power = nsAttackData::nsBodyBlow::POWER; // UŒ‚—Í
        m_attackTimeLimit = nsAttackData::nsBodyBlow::ATTACK_TIME_LIMIT; // UŒ‚ŠÔ
        m_attackType = EnAttackType::enBodyBlow; // UŒ‚‚Ìí—Ş
        //m_impactType = nsAttackData::nsBodyBlow::IMPACT_TYPE; // UŒ‚‚É‚æ‚é‰e‹¿
    }
    // ƒXƒgƒŒ[ƒg
    else if (EnAttackType::enStraight == attackType) {
        m_power = nsAttackData::nsStraight::POWER; // UŒ‚—Í
        m_attackTimeLimit = nsAttackData::nsStraight::ATTACK_TIME_LIMIT; // UŒ‚ŠÔ
        m_attackType = EnAttackType::enStraight; // UŒ‚‚Ìí—Ş
        //m_impactType = nsAttackData::nsStraight::IMPACT_TYPE; // UŒ‚‚É‚æ‚é‰e‹¿
    }

    m_flagAttackNow = true; // Œ»İUŒ‚’†‚©

    return true;
}

void StAttackData::Update()
{
    if (false == m_flagAttackNow) {
        return;
    }

    // UŒ‚‚µ‚Ä‚¢‚éÅ’†‚©H
    // UŒ‚ƒtƒŒ[ƒ€‚ği‚ß‚é
    if (false == AttackFrameUpdate()) {
        // UŒ‚‚ªI‚í‚Á‚Ä‚¢‚È‚¢
        return;
    }

    // UŒ‚‚ªI‚í‚Á‚½ó‘Ô‚É‚È‚Á‚½‚©H
    // UŒ‚”»’è‚ğíœ
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