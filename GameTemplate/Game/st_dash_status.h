#pragma once

/**
 * @brief ダッシュに関する構造体
*/
struct StDashStatus
{
public: // function
    /**
     * @brief ダッシュを始めるときの処理
    */
    void StartDash();

    /**
     * @brief ダッシュに関する毎フレームの処理
    */
    void DashUpdate();

    /**
     * @brief ダッシュの残り回数の回復の処理
    */
    void DashRecoveryTime();


public: // Get function
    const bool GetFlagDash()
    {
        return m_flagDash;
    }

    const int GetRemainingNumberOfTimes()
    {
        return m_remainingNumberOfTimes;
    }


private: // constant
    const int m_MAX_COUNT_DASH = 5; // ダッシュしている時間の上限
    const int m_MAX_RECOVERY_TIME = 50; // 残り回数の回復時間の上限
    const int m_MAX_REMAINING_NUMBER_OF_TIMES = 3; // ダッシュの残り回数の上限

private: // data member
    bool m_flagDash = false; // ダッシュ中か
    int m_countDash = 0; // ダッシュしている時間
    int m_remainingNumberOfTimes = m_MAX_REMAINING_NUMBER_OF_TIMES; // ダッシュの残り回数
    int m_countRecoveryTime = 0; // 残り回数の回復時間
};