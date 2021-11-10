#pragma once

/**
 * @brief �U�����Ɋւ���\����
*/
struct StAttackData
{
public: // function
    void SetAttackData(const int attackType);

    /**
     * @brief �U���͈͂̈ʒu�����쐬
     * �ʒu���v���C���[�̑O���̂Ƃ��Ɏg�p
     * @param playerPosition �v���C���[�̈ʒu
     * @param playerRotation �v���C���[�̉�]
     * @return �U���͈͂̈ʒu
    */
    const Vector3& CreateAttackPosition(const Vector3& playerPosition, const Quaternion& playerRotation);
    

    const bool UpdateFinish();


    void UpdateContinuousAttack();

    /**
     * @brief �U�����n�߂�܂ł̃f�B���C�̏���
    */
    const bool DelayAttack();

    /**
     * @brief �U���������Z�b�g
    */
    void ResetAttackData();

    /**
     * @brief �U�������������Ƃ��̍U���f�[�^�̊m�F
    */
    void HitCheckAttackData();



public: // Get function
    const Vector3& GetRange()
    {
        return m_range;
    }

    const bool GetFlagCreateAttackRange()
    {
        return m_flagCreateAttackRange;
    }

    const bool GetFlagAttackNow()
    {
        return m_flagAttackNow;
    }

    const int GetPower()
    {
        return m_power;
    }

    const bool GetFlagAlreadyAttacked()
    {
        return m_flagAlreadyAttacked;
    }

    const int GetImpactType()
    {
        return m_impactType;
    }

    const int GetAttackType()
    {
        return m_attackType;
    }

    const bool GetFlagNextAttackPossible()
    {
        return m_flagNextAttackPossible;
    }


public: // Set function
    void SetFlagAlreadyAttacked(const bool flag)
    {
        m_flagAlreadyAttacked = flag;
    }

    void SetFlagNextAttackPossible(const bool flag)
    {
        m_flagNextAttackPossible = flag;
    }


public: // enum
    /**
     * @brief �U���̎��
    */
    enum EnAttackType
    {
        enNotAttacking, // �U�����Ă��Ȃ�
        enJub, // �W���u
        enHook,
        enBodyBlow,
        enStraight,
        enUppercut, // �A�b�p�[�J�b�g
    };

    /**
     * @brief �U���ɂ��e��
    */
    enum EnImpactType
    {
        enNotImpact, // �Ȃɂ��Ȃ�
        enKnockBack, // �m�b�N�o�b�N
        enDown // �_�E��
    };


private: // data member
    // status
    int m_power = 0; // �U����
    int m_attackType = EnAttackType::enNotAttacking; // �U���̎��
    int m_impactType = EnImpactType::enNotImpact; // �U���ɂ��e��
    int m_nextNGAttackType = EnAttackType::enNotAttacking; // �U���̏�񂪃��Z�b�g�����܂ŁA���Ɏg�p�ł��Ȃ��U��
    // attack time
    int m_attackTime = 0; // �U������
    int m_attackTimeLimit = 0; // �U�����Ԃ̏��
    // delay time
    int m_delayTime = 0; // �U���܂ł̃f�B���C
    int m_delayTimeLimit = 0; // �U���܂ł̃f�B���C�̏��
    bool m_flagFinishDelay = true; // �U���J�n�܂ł̃f�B���C���I��������i�f�B���C���Ȃ��ꍇ[true]�ɐݒ�j
    // attack range
    Vector3 m_range = Vector3::Zero; //�U���͈�
    float m_positionUpY = 0.0f;
    float m_positionUpZ = 0.0f;
    // continuous attack
    int m_countContinuousAttack = 0; // �A���U���̉�
    int m_maxCountContinuousAttack = 0; // �A���U���̍ő吔
    int m_continuousAttackGraceTime = 0; // �A���U���̗P�\����
    int m_continuousAttackGraceTimeLimit = 0; // �A���U���̗P�\���Ԃ̏��
    bool m_flagContinuousAttack = false; // �A���U������
    // flag
    bool m_flagStartAttack = false; // �U�����J�n������
    bool m_flagAlreadyAttacked = false; // �U���������������Ă��邩
    bool m_flagAttackNow = false; // ���ݍU������
    bool m_flagCreateAttackRange = false; // �U��������쐬������
    bool m_flagNextAttackPossible = true; // ���̍U�����ł��邩
};