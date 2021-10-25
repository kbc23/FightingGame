#include "stdafx.h"
#include "st_attack_data.h"



namespace nsAttackData
{
    ////////////////////////////////////////////////////////////
    // �ʏ�U��
    ////////////////////////////////////////////////////////////

    namespace nsNormalAttack
    {
        const int POWER = 50; // �U����
        const int ATTACK_TIME_LIMIT = 15; // �U������
        const Vector3 RANGE = { 100.0f,50.0f,50.0f }; // �U���͈�
        const float POSITION_UP_Y = 50.0f; // �U���͈͂�Y���W�̒���
    }

    ////////////////////////////////////////////////////////////
    // �T�u�U��
    ////////////////////////////////////////////////////////////

    namespace nsSubAttack
    {
        const int POWER = 100; // �U����
        const int ATTACK_TIME_LIMIT = 30; // �U������
        const int DELAY_TIME_LIMIT = 20; // �U���܂ł̃f�B���C
        const Vector3 RANGE = { 50.0f,50.0f,500.0f }; // �U���͈�
        const float POSITION_UP_Y = 50.0f; // �U���͈͂�Y���W�̒���
    }
}



void StAttackData::SetAttackData(const int attackType)
{
    // �ʏ�U��
    if (EnAttackType::enNormal == attackType) {
        m_power = nsAttackData::nsNormalAttack::POWER;
        m_attackTimeLimit = nsAttackData::nsNormalAttack::ATTACK_TIME_LIMIT;
        m_range = nsAttackData::nsNormalAttack::RANGE;
        m_positionUpY = nsAttackData::nsNormalAttack::POSITION_UP_Y;
        m_flagAttackNow = true;
        m_attackType = EnAttackType::enNormal;
    }
    // �T�u�U��
    else if (EnAttackType::enSub == attackType) {
        m_power = nsAttackData::nsSubAttack::POWER;
        m_attackTimeLimit = nsAttackData::nsSubAttack::ATTACK_TIME_LIMIT;
        m_delayTimeLimit = nsAttackData::nsSubAttack::DELAY_TIME_LIMIT;
        m_flagFinishDelay = false;
        m_range = nsAttackData::nsSubAttack::RANGE;
        m_positionUpY = nsAttackData::nsSubAttack::POSITION_UP_Y;
        m_flagAttackNow = true;
        m_attackType = EnAttackType::enSub;
    }
}

const Vector3& StAttackData::CreateAttackPosition(const Vector3& playerPosition, const Quaternion& playerRotation)
{
    // ��������U���͈͂̃|�W�V����
    // �O�����ɍU���͈͂̑O�����̔����̒l��ǉ�����
    Vector3 attackRangePosition =
    {
        playerPosition.x,
        playerPosition.y,
        playerPosition.z - m_range.z / 2 - 20.0f // �J�����̑O�����ɍU���͈͂̑O�����̔����̒l��ǉ�
    };
    // �L�����N�^�[�̃|�W�V����
    Vector3 local_playerPosition = playerPosition;

    // �U���͈͂̃|�W�V��������L�����N�^�[�̃|�W�V�����̃x�N�g�����擾
    Vector3 toPos = attackRangePosition - local_playerPosition;
    // �L�����N�^�[��Quaternion���g���ăx�N�g�����v���C���[�̑O�����ɉ�]������
    playerRotation.Apply(toPos);

    // ��L�Ŏ擾������񂩂�A�U���͈͂𐶐�����|�W�V�������擾
    local_playerPosition = local_playerPosition - toPos;

    // �w�肵���l����Y���W���㏸����
    local_playerPosition.y = m_positionUpY;

    // �U��������쐬��������ɂ���
    m_flagCreateAttackRange = true;

    return local_playerPosition;
}

const bool StAttackData::UpdateFinish()
{
    ++m_attackTime;

    if (m_attackTimeLimit <= m_attackTime) {
        return true;
    }

    return false;
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
    m_power = 0;
    m_attackTime = 0;
    m_attackTimeLimit = 0;
    m_delayTime = 0;
    m_delayTimeLimit = 0;
    m_flagFinishDelay = true;
    m_range = Vector3::Zero;
    m_positionUpY = 0.0f;
    m_flagAlreadyAttacked = false;
    m_flagAttackNow = false;
    m_flagCreateAttackRange = false;
}
