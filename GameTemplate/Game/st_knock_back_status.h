#pragma once

/**
 * @brief ノックバックに関する構造体
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
    int m_knockBackTime = 0; // ノックバックしている時間
    int m_knockBackTimeLimit = 30; // ノックバックしている時間の上限
    bool m_flagKnockBack = false; // ノックバックしているか（ノックバック中は操作不可）
};