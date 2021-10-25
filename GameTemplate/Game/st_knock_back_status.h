#pragma once

/**
 * @brief �m�b�N�o�b�N�Ɋւ���\����
*/
struct StKnockBackStatus
{
public: // function
    void StartKnockBack();

    void KnockBackUpdate();


public: // Get function
    const bool GetFlagKnockBack()
    {
        return m_flagKnockBack;
    }



private: // data member
    int m_knockBackTime = 0; // �m�b�N�o�b�N���Ă��鎞��
    int m_knockBackTimeLimit = 30; // �m�b�N�o�b�N���Ă��鎞�Ԃ̏��
    bool m_flagKnockBack = false; // �m�b�N�o�b�N���Ă��邩�i�m�b�N�o�b�N���͑���s�j
};