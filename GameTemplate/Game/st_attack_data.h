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
        enHook, // �t�b�N
        enBodyBlow, // �{�f�B�u���[
        enStraight, // �X�g���[�g
        enUppercut, // �A�b�p�[�J�b�g
    };

    /**
     * @brief �U���ɂ��e��
    */
    enum EnImpactType
    {
        enNotImpact, // �Ȃɂ��Ȃ�
        enSqueeze, // �̂�����
        enDown // �_�E��
    };


private: // data member
    ////////////////////////////////////////////////////////////
    // �X�e�[�^�X
    ////////////////////////////////////////////////////////////

    int m_power = 0; // �U����
    int m_attackType = EnAttackType::enNotAttacking; // �U���̎��
    int m_impactType = EnImpactType::enNotImpact; // �U���ɂ��e��
    // �U���̏�񂪃��Z�b�g�����܂ŁA���ɋZ�L�����Z�������Ďg�p�ł��Ȃ��U��
    int m_nextNGAttackType = EnAttackType::enNotAttacking;
    // �U���̏�񂪃��Z�b�g�����܂ŁA���ɗB��Z�L�����Z�������Ďg�p�ł���U��
    int m_nextOnlyOKAttackType = EnAttackType::enNotAttacking;
    // �������̗D��x ([m_nextOnlyOKAttackType] > [m_nextNGAttackType])

    ////////////////////////////////////////////////////////////
    // �U������
    ////////////////////////////////////////////////////////////

    int m_attackTime = 0; // �U������
    int m_attackTimeLimit = 0; // �U�����Ԃ̏��
    
    ////////////////////////////////////////////////////////////
    // �U���̃f�B���C
    ////////////////////////////////////////////////////////////

    int m_delayTime = 0; // �U���܂ł̃f�B���C
    int m_delayTimeLimit = 0; // �U���܂ł̃f�B���C�̏��
    bool m_flagFinishDelay = true; // �U���J�n�܂ł̃f�B���C���I��������i�f�B���C���Ȃ��ꍇ[true]�ɐݒ�j
    
    ////////////////////////////////////////////////////////////
    // �U���͈�
    ////////////////////////////////////////////////////////////

    Vector3 m_range = Vector3::Zero; //�U���͈�
    float m_positionUpY = 0.0f;
    float m_positionUpZ = 0.0f;

    ////////////////////////////////////////////////////////////
    // �A���U��
    ////////////////////////////////////////////////////////////

    int m_countContinuousAttack = 0; // �A���U���̉�
    //int m_maxCountContinuousAttack = 10; // �A���U���̍ő吔
    static const int m_MAX_COUNT_CONTINUOUS_ATTACK = 4; // �A���U���̍ő吔 - 1
    int m_continuousAttackGraceTime = 0; // �A���U���̗P�\����
    //int m_continuousAttackGraceTimeLimit = 0; // �A���U���̗P�\���Ԃ̏��
    static const int m_CONTINUOUS_ATTACK_GRACE_TIME_LIMIT = 90; // �A���U���̗P�\���Ԃ̏��
    bool m_flagContinuousAttack = false; // �A���U������

    ////////////////////////////////////////////////////////////
    // �A���U�����̃W���u�Ɋ֌W����f�[�^
    ////////////////////////////////////////////////////////////

    int m_continueJubAttack = 0; // �A���U���œ��������W���u�̐�
    static const int m_MAX_CONTINUE_JUB_ATTACK = 2; // �A���U���Ŏg�p�ł���W���u�̍ő吔 - 1

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagStartAttack = false; // �U�����J�n������
    bool m_flagAlreadyAttacked = false; // �U���������������Ă��邩
    bool m_flagAttackNow = false; // ���ݍU������
    bool m_flagCreateAttackRange = false; // �U��������쐬������
    bool m_flagNextAttackPossible = true; // ���̍U�����ł��邩
};