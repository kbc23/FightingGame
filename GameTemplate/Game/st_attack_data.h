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

    /**
     * @brief �U�����n�߂�܂ł̃f�B���C�̏���
    */
    const bool DelayAttack();

    /**
     * @brief �U���������Z�b�g
    */
    void ResetAttackData();



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


public: // Set function
    void SetFlagAlreadyAttacked(const bool flag)
    {
        m_flagAlreadyAttacked = flag;
    }


public:
    /**
     * @brief �U���̎��
    */
    enum EnAttackType
    {
        enNotAttacking, // �U�����Ă��Ȃ�
        enNormal, // �ʏ�U��
        enSub, // �T�u�U��
    };


private: // data member
    // status
    int m_power = 0; // �U����
    int m_attackType = EnAttackType::enNotAttacking; // �U���̎��
    // attack time
    int m_attackTime = 0; // �U������
    int m_attackTimeLimit = 0; // �U�����Ԃ̏��
    // delay time
    int m_delayTime = 0; // �U���܂ł̃f�B���C
    int m_delayTimeLimit = 0; // �U���܂ł̃f�B���C�̏��
    bool m_flagFinishDelay = true; // �U���J�n�܂ł̃f�B���C���I��������i�f�B���C���Ȃ��ꍇ[true]�ɐݒ�j
    // attack range
    Vector3 m_range = Vector3::Zero; //�U���͈�
    float m_positionUpY = 0.0f; // �㏸������U���͈͂�Y���W�̗�
    // flag
    bool m_flagAlreadyAttacked = false; // �U���������������Ă��邩
    bool m_flagAttackNow = false; // ���ݍU������
    bool m_flagCreateAttackRange = false; // �U��������쐬������
};