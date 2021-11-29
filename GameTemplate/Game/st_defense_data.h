#pragma once


struct StDefenseData
{
public:
    void Update();


public: // Get function
    /**
     * @brief –hŒä’†‚©æ“¾
     * @return –hŒä’†‚©
    */
    const bool GetFlagDefense()
    {
        return m_flagDefense;
    }
    

public: // Set function
    /**
     * @brief –hŒä’†‚©ƒZƒbƒg
     * @param defense –hŒä’†‚©
    */
    void SetFlagDefense(const bool defense)
    {
        m_flagDefense = defense;
    }


private: // data member
    bool m_flagDefense = false; // –hŒä’†‚©
};