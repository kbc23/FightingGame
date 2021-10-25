#pragma once

/**
 * @brief �_�E���Ɋւ���\����
*/
struct StDownStatus
{
public: // function
    void StartDown();

    void DownUpdate();


public: // Get function
    const bool GetFlagDown()
    {
        return m_flagDown;
    }


private: // data member
    int m_downTime = 0; // �_�E�����Ă��鎞��
    int m_downTimeLimit = 60; // �_�E�����Ă��鎞�Ԃ̏��
    bool m_flagDown = false; // �_�E�����Ă��邩�i�_�E�����͑���s�A���G�j
    // �N���オ��������������G���Ԃ��~�����i���G���Ԃ���I�H�j
};