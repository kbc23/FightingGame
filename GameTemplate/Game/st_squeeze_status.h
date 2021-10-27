#pragma once

/**
 * @brief �̂�����Ɋւ���\����
*/
struct StSqueezeStatus
{
public: // function
    void StartSqueeze();

    void SqueezeUpdate();


public: // Get function
    const bool GetFlagSqueeze()
    {
        return m_flagSqueeze;
    }



private: // data member
    int m_squeezeTime = 0; // �m�b�N�o�b�N���Ă��鎞��
    int m_squeezeTimeLimit = 25; // �m�b�N�o�b�N���Ă��鎞�Ԃ̏��
    bool m_flagSqueeze = false; // �m�b�N�o�b�N���Ă��邩�i�m�b�N�o�b�N���͑���s�j
};