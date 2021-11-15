#pragma once

/**
 * @brief �_�E���Ɋւ���\����
*/
struct StDownStatus
{
public: // function
    void StartDown();

    void DownUpdate();


private:
    void DownCount();

    void InvincibleCount();


public: // Get function
    const bool GetFlagDown()
    {
        return m_flagDown;
    }

    const bool GetFlagInvincible()
    {
        return m_flagInvincible;
    }


private: //constant
    static const int m_MAX_CONTINUE_JUB_ATTACK = 3;


private: // data member
    ////////////////////////////////////////////////////////////
    // �_�E���֘A
    ////////////////////////////////////////////////////////////

    int m_downTime = 0; // �_�E�����Ă��鎞��
    int m_downTimeLimit = 60; // �_�E�����Ă��鎞�Ԃ̏��
    bool m_flagDown = false; // �_�E�����Ă��邩�i�_�E�����͑���s�A���G�j
    
    ////////////////////////////////////////////////////////////
    // ���G���Ԋ֘A
    ////////////////////////////////////////////////////////////

    int m_invincibleTime = 0; // ���G����
    int m_invincibleTimeLimit = 120; // ���G���Ԃ̏��
    bool m_flagInvincible = false; // ���G��Ԃ�
};