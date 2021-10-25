#pragma once

/**
 * @brief �_�b�V���Ɋւ���\����
*/
struct StDashStatus
{
public: // function
    /**
     * @brief �_�b�V�����n�߂�Ƃ��̏���
    */
    void StartDash();

    /**
     * @brief �_�b�V���Ɋւ��閈�t���[���̏���
    */
    void DashUpdate();

    /**
     * @brief �_�b�V���̎c��񐔂̉񕜂̏���
    */
    void DashRecoveryTime();


public: // Get function
    const bool GetFlagDash()
    {
        return m_flagDash;
    }

    const int GetRemainingNumberOfTimes()
    {
        return m_remainingNumberOfTimes;
    }


private: // constant
    const int m_MAX_COUNT_DASH = 5; // �_�b�V�����Ă��鎞�Ԃ̏��
    const int m_MAX_RECOVERY_TIME = 50; // �c��񐔂̉񕜎��Ԃ̏��
    const int m_MAX_REMAINING_NUMBER_OF_TIMES = 3; // �_�b�V���̎c��񐔂̏��

private: // data member
    bool m_flagDash = false; // �_�b�V������
    int m_countDash = 0; // �_�b�V�����Ă��鎞��
    int m_remainingNumberOfTimes = m_MAX_REMAINING_NUMBER_OF_TIMES; // �_�b�V���̎c���
    int m_countRecoveryTime = 0; // �c��񐔂̉񕜎���
};