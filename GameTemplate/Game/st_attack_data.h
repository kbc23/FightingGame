#pragma once

/**
 * @brief �U�����Ɋւ���\����
*/
struct StAttackData
{
public: // function
    /**
     * @brief �U�������Z�b�g
     * @param attackType �U���^�C�v
     * @return �����Z�b�g������
    */
    const bool SetAttackData(const int attackType);

    /**
     * @brief ���t���[���̏���
    */
    void Update();


private:
    /**
     * @brief �U���t���[���̍X�V
     * @return �U�����I�������
    */
    const bool AttackFrameUpdate();

    /**
     * @brief �U���̏I������
    */
    void AttackFinish();


public: // get function
    /**
     * @brief ���ݍU���������擾
     * @return ���ݍU������
    */
    const bool GetFlagAttackNow() const
    {
        return m_flagAttackNow;
    }

    /**
     * @brief �U���͂��擾
     * @return �U����
    */
    const int GetPower() const
    {
        return m_power;
    }

    /**
     * @brief �U�������łɓ������������擾
     * @return �U�������łɓ���������
    */
    const bool GetFlagAlreadyAttacked() const
    {
        return m_flagAlreadyAttacked;
    }

    /**
     * @brief �U���̎�ނ��擾
     * @return �U���̎��
    */
    const int GetAttackType() const
    {
        return m_attackType;
    }


public: // Set function
    /**
     * @brief �U�������łɓ������������Z�b�g
     * @param flag �U�������łɓ���������
    */
    void SetFlagAlreadyAttacked(const bool flag)
    {
        m_flagAlreadyAttacked = flag;
    }

    /**
     * @brief �U�������Ă��Ȃ���ԂɃZ�b�g
    */
    void SetFalseFlagAttackNow()
    {
        m_flagAttackNow = false;
    }


public: // enum
    /**
     * @brief �U���̎��
    */
    enum EnAttackType
    {
        enNotAttacking, // �U�����Ă��Ȃ�
        enJub,          // �W���u
        enHook,         // �t�b�N
        enBodyBlow,     // �{�f�B�u���[
        enStraight,     // �X�g���[�g
        enUppercut,     // �A�b�p�[�J�b�g
    };

    /**
     * @brief �U���ɂ��e��
    */
    enum EnImpactType
    {
        enNotImpact,    // �Ȃɂ��Ȃ�
        enSqueeze,      // �̂�����
        enDown          // �_�E��
    };


private: // data member
    ////////////////////////////////////////////////////////////
    // �X�e�[�^�X
    ////////////////////////////////////////////////////////////

    int m_power = 0; // �U����
    int m_attackType = EnAttackType::enNotAttacking; // �U���̎��
    //int m_impactType = EnImpactType::enNotImpact; // �U���ɂ��e��

    ////////////////////////////////////////////////////////////
    // �U������
    ////////////////////////////////////////////////////////////

    int m_attackTime = 0; // �U������
    int m_attackTimeLimit = 0; // �U�����Ԃ̏��

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagAlreadyAttacked = false; // �U���������������Ă��邩
    bool m_flagAttackNow = false; // ���ݍU������
};