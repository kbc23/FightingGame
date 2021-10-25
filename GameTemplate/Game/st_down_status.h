#pragma once

/**
 * @brief ダウンに関する構造体
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
    int m_downTime = 0; // ダウンしている時間
    int m_downTimeLimit = 60; // ダウンしている時間の上限
    bool m_flagDown = false; // ダウンしているか（ダウン中は操作不可、無敵）
    // 起き上がった後も少し無敵時間が欲しい（無敵時間さん！？）
};