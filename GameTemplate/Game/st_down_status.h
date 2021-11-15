#pragma once

/**
 * @brief ダウンに関する構造体
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
    // ダウン関連
    ////////////////////////////////////////////////////////////

    int m_downTime = 0; // ダウンしている時間
    int m_downTimeLimit = 60; // ダウンしている時間の上限
    bool m_flagDown = false; // ダウンしているか（ダウン中は操作不可、無敵）
    
    ////////////////////////////////////////////////////////////
    // 無敵時間関連
    ////////////////////////////////////////////////////////////

    int m_invincibleTime = 0; // 無敵時間
    int m_invincibleTimeLimit = 120; // 無敵時間の上限
    bool m_flagInvincible = false; // 無敵状態か
};