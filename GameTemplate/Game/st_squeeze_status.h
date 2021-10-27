#pragma once

/**
 * @brief のけぞりに関する構造体
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
    int m_squeezeTime = 0; // ノックバックしている時間
    int m_squeezeTimeLimit = 25; // ノックバックしている時間の上限
    bool m_flagSqueeze = false; // ノックバックしているか（ノックバック中は操作不可）
};