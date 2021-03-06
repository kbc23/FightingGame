#pragma once


struct StDefenseData
{
public:
    void Update();


public: // Get function
    /**
     * @brief 防御中か取得
     * @return 防御中か
    */
    const bool GetFlagDefense()
    {
        return m_flagDefense;
    }
    

public: // Set function
    /**
     * @brief 防御中かセット
     * @param defense 防御中か
    */
    void SetFlagDefense(const bool defense)
    {
        m_flagDefense = defense;
    }


private: // data member
    bool m_flagDefense = false; // 防御中か
};