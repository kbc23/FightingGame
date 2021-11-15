#include "stdafx.h"
#include "st_attack_data.h"



namespace nsAttackData
{
    // �W���u
    namespace nsJub
    {
        const int POWER = 10; // �U����
        const int ATTACK_TIME_LIMIT = 16; // �U������
        const int DELAY_TIME_LIMIT = 4; // �U���܂ł̃f�B���C
        const Vector3 RANGE = { 25.0f,25.0f,50.0f }; // �U���͈�
        const float POSITION_UP_Y = 100.0f; // �U���͈͂�Y���W�̒���
        const float POSITION_UP_Z = 0.0f; // �U���͈͂�Z���W�̒���
        const int MAX_CONTINUOUS_ATTACK_COUNT = 3; // �A���U���̍ő吔
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // �U���ɂ��e��
    }
    
    // �A�b�p�[�J�b�g
    namespace nsUppercut
    {
        const int POWER = 15; // �U����
        const int ATTACK_TIME_LIMIT = 15; // �U������
        const int DELAY_TIME_LIMIT = 7; // �U���܂ł̃f�B���C
        const Vector3 RANGE = { 25.0f,40.0f,35.0f }; // �U���͈�
        const float POSITION_UP_Y = 90.0f; // �U���͈͂�Y���W�̒���
        const float POSITION_UP_Z = 25.0f; // �U���͈͂�Z���W�̒���
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // �U���ɂ��e��
    }

    // �t�b�N
    namespace nsHook
    {
        const int POWER = 20; // �U����
        const int ATTACK_TIME_LIMIT = 15; // �U������
        const int DELAY_TIME_LIMIT = 8; // �U���܂ł̃f�B���C
        const Vector3 RANGE = { 50.0f,20.0f,30.0f }; // �U���͈�
        const float POSITION_UP_Y = 100.0f; // �U���͈͂�Y���W�̒���
        const float POSITION_UP_Z = 25.0f; // �U���͈͂�Z���W�̒���
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // �U���ɂ��e��
    }

    // �{�f�B�u���[
    namespace nsBodyBlow
    {
        const int POWER = 25; // �U����
        const int ATTACK_TIME_LIMIT = 21; // �U������
        const int DELAY_TIME_LIMIT = 19; // �U���܂ł̃f�B���C
        const Vector3 RANGE = { 20.0f,20.0f,35.0f }; // �U���͈�
        const float POSITION_UP_Y = 75.0f; // �U���͈͂�Y���W�̒���
        const float POSITION_UP_Z = 30.0f; // �U���͈͂�Z���W�̒���
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // �U���ɂ��e��
    }

    // �X�g���[�g
    namespace nsStraight
    {
        const int POWER = 50; // �U����
        const int ATTACK_TIME_LIMIT = 22; // �U������
        const int DELAY_TIME_LIMIT = 19; // �U���܂ł̃f�B���C
        const Vector3 RANGE = { 25.0f,20.0f,85.0f }; // �U���͈�
        const float POSITION_UP_Y = 85.0f; // �U���͈͂�Y���W�̒���
        const float POSITION_UP_Z = 0.0f; // �U���͈͂�Z���W�̒���
        const int IMPACT_TYPE = StAttackData::EnImpactType::enDown; // �U���ɂ��e��
    }
}



const bool StAttackData::SetAttackData(const int attackType)
{
    if (m_nextNGAttackType == attackType) {
        return false;
    }

    // �W���u
    if (EnAttackType::enJub == attackType) {
        m_power = nsAttackData::nsJub::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsJub::ATTACK_TIME_LIMIT; // �U������
        m_delayTimeLimit = nsAttackData::nsJub::DELAY_TIME_LIMIT; // �U���܂ł̃f�B���C
        m_range = nsAttackData::nsJub::RANGE; // �U���͈�
        m_positionUpY= nsAttackData::nsJub::POSITION_UP_Y; // �U���͈͂�Y���W�̒���
        m_positionUpZ = nsAttackData::nsJub::POSITION_UP_Z; // �U���͈͂�Z���W�̒���
        m_attackType = EnAttackType::enJub; // �U���̎��
        m_impactType = nsAttackData::nsJub::IMPACT_TYPE; // �U���ɂ��e��
    }
    // �A�b�p�[�J�b�g
    else if (EnAttackType::enUppercut == attackType) {
        m_power = nsAttackData::nsUppercut::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsUppercut::ATTACK_TIME_LIMIT; // �U������
        m_delayTimeLimit = nsAttackData::nsUppercut::DELAY_TIME_LIMIT; // �U���܂ł̃f�B���C
        m_range = nsAttackData::nsUppercut::RANGE; // �U���͈�
        m_positionUpY = nsAttackData::nsUppercut::POSITION_UP_Y; // �U���͈͂�Y���W�̒���
        m_positionUpZ = nsAttackData::nsUppercut::POSITION_UP_Z; // �U���͈͂�Z���W�̒���
        m_attackType = EnAttackType::enUppercut; // �U���̎��
        m_impactType = nsAttackData::nsUppercut::IMPACT_TYPE; // �U���ɂ��e��
    }
    // �t�b�N
    else if (EnAttackType::enHook == attackType) {
        m_power = nsAttackData::nsHook::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsHook::ATTACK_TIME_LIMIT; // �U������
        m_delayTimeLimit = nsAttackData::nsHook::DELAY_TIME_LIMIT; // �U���܂ł̃f�B���C
        m_range = nsAttackData::nsHook::RANGE; // �U���͈�
        m_positionUpY = nsAttackData::nsHook::POSITION_UP_Y; // �U���͈͂�Y���W�̒���
        m_positionUpZ = nsAttackData::nsHook::POSITION_UP_Z; // �U���͈͂�Z���W�̒���
        m_attackType = EnAttackType::enHook; // �U���̎��
        m_impactType = nsAttackData::nsHook::IMPACT_TYPE; // �U���ɂ��e��
    }
    // �{�f�B�u���[
    else if (EnAttackType::enBodyBlow == attackType) {
        m_power = nsAttackData::nsBodyBlow::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsBodyBlow::ATTACK_TIME_LIMIT; // �U������
        m_delayTimeLimit = nsAttackData::nsBodyBlow::DELAY_TIME_LIMIT; // �U���܂ł̃f�B���C
        m_range = nsAttackData::nsBodyBlow::RANGE; // �U���͈�
        m_positionUpY = nsAttackData::nsBodyBlow::POSITION_UP_Y; // �U���͈͂�Y���W�̒���
        m_positionUpZ = nsAttackData::nsBodyBlow::POSITION_UP_Z; // �U���͈͂�Z���W�̒���
        m_attackType = EnAttackType::enBodyBlow; // �U���̎��
        m_impactType = nsAttackData::nsBodyBlow::IMPACT_TYPE; // �U���ɂ��e��
    }
    // �X�g���[�g
    else if (EnAttackType::enStraight == attackType) {
        m_power = nsAttackData::nsStraight::POWER; // �U����
        m_attackTimeLimit = nsAttackData::nsStraight::ATTACK_TIME_LIMIT; // �U������
        m_delayTimeLimit = nsAttackData::nsStraight::DELAY_TIME_LIMIT; // �U���܂ł̃f�B���C
        m_range = nsAttackData::nsStraight::RANGE; // �U���͈�
        m_positionUpY = nsAttackData::nsStraight::POSITION_UP_Y; // �U���͈͂�Y���W�̒���
        m_positionUpZ = nsAttackData::nsStraight::POSITION_UP_Z; // �U���͈͂�Z���W�̒���
        m_attackType = EnAttackType::enStraight; // �U���̎��
        m_impactType = nsAttackData::nsStraight::IMPACT_TYPE; // �U���ɂ��e��
    }

    m_flagAttackNow = true; // ���ݍU������
    m_flagStartAttack = true; // �U�����J�n������
    m_flagNextAttackPossible = false; // ���̍U�����ł��邩
    m_flagFinishDelay = false; // �f�B���C���I�������

    // �A���U���Ɋւ��鏈���i�A���U���T��ڂ̂Ƃ��j
    if (m_MAX_COUNT_CONTINUOUS_ATTACK <= m_countContinuousAttack) {
        m_impactType = EnImpactType::enDown;
    }

    // �A���U�����̃W���u���R��ڂ̂Ƃ�
    if (m_attackType == EnAttackType::enJub && m_MAX_CONTINUE_JUB_ATTACK <= m_continueJubAttack) {
        m_impactType = EnImpactType::enDown;
    }

    m_nextNGAttackType = EnAttackType::enNotAttacking;

    return true;
}

const Vector3& StAttackData::CreateAttackPosition(const Vector3& playerPosition, const Quaternion& playerRotation)
{
    // ��������U���͈͂̃|�W�V����
    // �O�����ɍU���͈͂̑O�����̔����̒l��ǉ�����
    Vector3 attackRangePosition =
    {
        playerPosition.x,
        playerPosition.y,
        // �U���͈͂̑O�����̔����̒l�Ǝw�肵���l����Z���W�����炷
        playerPosition.z - m_range.z / 2 - 20.0f - m_positionUpZ
    };
    // �L�����N�^�[�̃|�W�V����
    Vector3 local_playerPosition = playerPosition;

    // �U���͈͂̃|�W�V��������L�����N�^�[�̃|�W�V�����̃x�N�g�����擾
    Vector3 toPos = attackRangePosition - local_playerPosition;
    // �L�����N�^�[��Quaternion���g���ăx�N�g�����v���C���[�̑O�����ɉ�]������
    playerRotation.Apply(toPos);

    // ��L�Ŏ擾������񂩂�A�U���͈͂𐶐�����|�W�V�������擾
    attackRangePosition = local_playerPosition - toPos;

    // �w�肵���l����Y���W���㏸����
    attackRangePosition.y = m_positionUpY;

    // �U��������쐬��������ɂ���
    m_flagCreateAttackRange = true;

    return attackRangePosition;
}

const bool StAttackData::UpdateFinish()
{
    ++m_attackTime;

    if (m_attackTimeLimit <= m_attackTime) {
        return true;
    }

    return false;
}

void StAttackData::UpdateContinuousAttack()
{
    if (false == m_flagContinuousAttack) {
        return;
    }

    ++m_continuousAttackGraceTime;

    if (m_CONTINUOUS_ATTACK_GRACE_TIME_LIMIT <= m_continuousAttackGraceTime) {
        m_continuousAttackGraceTime = 0;
        m_countContinuousAttack = 0;
        m_flagContinuousAttack = false;
        m_continueJubAttack = 0;
        m_nextNGAttackType = EnAttackType::enNotAttacking;
    }
}

const bool StAttackData::DelayAttack()
{
    if (true == m_flagFinishDelay) {
        return true;
    }

    ++m_delayTime;

    if (m_delayTimeLimit <= m_delayTime) {
        m_flagFinishDelay = true;

        return true;
    }

    return false;
}

void StAttackData::ResetAttackData()
{
    // �U�����̃X�e�[�^�X�̏�����
    m_power = 0; // �U���́@
    m_attackTime = 0; // �U������
    m_attackTimeLimit = 0; // �U�����Ԃ̏��
    m_delayTime = 0; // �U���܂ł̃f�B���C
    m_delayTimeLimit = 0; // �U���܂ł̃f�B���C�̏��
    m_flagFinishDelay = true; // �f�B���C���I�������
    m_range = Vector3::Zero; // �͈�
    m_flagAlreadyAttacked = false; // �U���������������Ă��邩
    m_flagAttackNow = false;
    m_flagCreateAttackRange = false;
    m_impactType = EnImpactType::enNotImpact;
}

void StAttackData::HitCheckAttackData()
{
    if (EnAttackType::enJub == m_attackType) {
        m_flagNextAttackPossible = true;
        ++m_continueJubAttack;
    }

    if (EnAttackType::enHook == m_attackType) {
        m_flagNextAttackPossible = true;
        m_nextNGAttackType = EnAttackType::enHook;
    }

    if (EnAttackType::enUppercut == m_attackType) {
        m_flagNextAttackPossible = true;
        m_nextNGAttackType = EnAttackType::enUppercut;
    }

    if (EnAttackType::enBodyBlow == m_attackType) {
        m_flagNextAttackPossible = true;
        m_nextNGAttackType = EnAttackType::enBodyBlow;
    }

    // �A���U���Ɋւ��鏈��
    ++m_countContinuousAttack;
    m_continuousAttackGraceTime = 0;
    m_flagContinuousAttack = true;
}