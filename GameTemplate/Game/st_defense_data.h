#pragma once


struct StDefenseData
{
public:
    void Update();


public: // Get function
    /**
     * @brief �h�䒆���擾
     * @return �h�䒆��
    */
    const bool GetFlagDefense()
    {
        return m_flagDefense;
    }
    

public: // Set function
    /**
     * @brief �h�䒆���Z�b�g
     * @param defense �h�䒆��
    */
    void SetFlagDefense(const bool defense)
    {
        m_flagDefense = defense;
    }


private: // data member
    bool m_flagDefense = false; // �h�䒆��
};