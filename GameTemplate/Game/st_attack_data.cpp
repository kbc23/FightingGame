#include "stdafx.h"
#include "st_attack_data.h"



namespace nsAttackData
{
    // �W���u
    namespace nsJub
    {
        const int POWER = 8; // �U����
        const int ATTACK_TIME_LIMIT = 16; // �U������
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // �U���ɂ��e��
    }
    
    // �A�b�p�[�J�b�g
    namespace nsUppercut
    {
        const int POWER = 16; // �U����
        const int ATTACK_TIME_LIMIT = 15; // �U������
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // �U���ɂ��e��
    }

    // �t�b�N
    namespace nsHook
    {
        const int POWER = 13; // �U����
        const int ATTACK_TIME_LIMIT = 15; // �U������
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // �U���ɂ��e��
    }

    // �{�f�B�u���[
    namespace nsBodyBlow
    {
        const int POWER = 25; // �U����
        const int ATTACK_TIME_LIMIT = 21; // �U������
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // �U���ɂ��e��
    }

    // �X�g���[�g
    namespace nsStraight
    {
        const int POWER = 30; // �U����
        const int ATTACK_TIME_LIMIT = 22; // �U������
        const int IMPACT_TYPE = StAttackData::EnImpactType::enDown; // �U���ɂ��e��
    }
}



const bool StAttackData::SetAttackData(const int attackType)
{
    // �W���u
    if (EnAttackType::enJub == attackType) {
        m_power = nsAttackData::nsJub::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsJub::ATTACK_TIME_LIMIT; // �U������
        m_attackType = EnAttackType::enJub; // �U���̎��
        //m_impactType = nsAttackData::nsJub::IMPACT_TYPE; // �U���ɂ��e��
    }
    // �A�b�p�[�J�b�g
    else if (EnAttackType::enUppercut == attackType) {
        m_power = nsAttackData::nsUppercut::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsUppercut::ATTACK_TIME_LIMIT; // �U������
        m_attackType = EnAttackType::enUppercut; // �U���̎��
        //m_impactType = nsAttackData::nsUppercut::IMPACT_TYPE; // �U���ɂ��e��
    }
    // �t�b�N
    else if (EnAttackType::enHook == attackType) {
        m_power = nsAttackData::nsHook::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsHook::ATTACK_TIME_LIMIT; // �U������
        m_attackType = EnAttackType::enHook; // �U���̎��
        //m_impactType = nsAttackData::nsHook::IMPACT_TYPE; // �U���ɂ��e��
    }
    // �{�f�B�u���[
    else if (EnAttackType::enBodyBlow == attackType) {
        m_power = nsAttackData::nsBodyBlow::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsBodyBlow::ATTACK_TIME_LIMIT; // �U������
        m_attackType = EnAttackType::enBodyBlow; // �U���̎��
        //m_impactType = nsAttackData::nsBodyBlow::IMPACT_TYPE; // �U���ɂ��e��
    }
    // �X�g���[�g
    else if (EnAttackType::enStraight == attackType) {
        m_power = nsAttackData::nsStraight::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsStraight::ATTACK_TIME_LIMIT; // �U������
        m_attackType = EnAttackType::enStraight; // �U���̎��
        //m_impactType = nsAttackData::nsStraight::IMPACT_TYPE; // �U���ɂ��e��
    }

    m_flagAttackNow = true; // ���ݍU������

    return true;
}

void StAttackData::Update()
{
    if (false == m_flagAttackNow) {
        return;
    }

    // �U�����Ă���Œ����H
    // �U���t���[����i�߂�
    if (false == AttackFrameUpdate()) {
        // �U�����I����Ă��Ȃ�
        return;
    }

    // �U�����I�������ԂɂȂ������H
    // �U��������폜
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