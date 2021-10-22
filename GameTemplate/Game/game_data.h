#pragma once

/**
 * @brief ゲームのいろいろなところで使用するデータを保存するクラス
 * ※注意！
 * 仕様上、いろいろなところで保存しなければならないものだけ入れること！
*/
class GameData : public IGameObject
{
public:
    GameData();
    ~GameData();
    bool Start() override final;
    void Update() override final;



public: // Get function
    /**
     * @brief プレイヤーの番号を取得
     * @return プレイヤーの番号
    */
    const int GetPlayerNum()
    {
        return m_playerNum;
    }

    /**
     * @brief 相手プレイヤーの番号を取得
     * @return 相手プレイヤーの番号
    */
    const int GetOtherPlayerNum()
    {
        return m_otherPlayerNum;
    }


public: // Set function
    /**
     * @brief プレイヤーの番号と相手プレイヤーの番号を保存
     * @param playerNum プレイヤーの番号
     * @param otherPlayerNum 相手プレイヤーの番号
    */
    void SetPlayerNumAndOtherPlayerNum(const int playerNum, const int otherPlayerNum)
    {
        m_playerNum = playerNum;
        m_otherPlayerNum = otherPlayerNum;
    }


private: // data member
    int m_playerNum = 0; // プレイヤーの番号
    int m_otherPlayerNum = 0; // 相手プレイヤーの番号


};