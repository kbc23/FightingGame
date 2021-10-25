#pragma once
#include "font_render.h"
#include "game_data.h"

/**
 * @brief プレイヤーに関するUIを管理するクラス
*/
class PlayerUI : public IGameObject
{
public:
    PlayerUI();
    ~PlayerUI();
    bool Start() override final;
    void Update() override final;



public: // Update data
    void UpdateHpUI(const int hp, const int playerNum)
    {
        m_hpUI->SetText(hp);
        SetPosition(playerNum);
    }

    void UpdateDashUI(const int dash, const int playerNum)
    {
        m_dashUI->SetText(dash);
        SetPosition(playerNum);
    }

    void UpdateKnockBackUI(const bool knockBack, const int playerNum)
    {
        if (true == knockBack) {
            m_knockBackUI->Activate();
        }
        else {
            m_knockBackUI->Deactivate();
        }

        SetPosition(playerNum);
    }

    void SetPosition(const int playerNum)
    {
        if (playerNum == m_findGameData->GetPlayerNum()) {
            m_hpUI->SetPositionX(-200.0f);
            m_dashUI->SetPosition({ -200.0f,-50.0f });
            m_knockBackUI->SetPosition({ -200.0f,-100.0f });
        }
        else if (playerNum == m_findGameData->GetOtherPlayerNum()) {
            m_hpUI->SetPositionX(200.0f);
            m_dashUI->SetPosition({ 200.0f,-50.0f });
            m_knockBackUI->SetPosition({ 200.0f,-100.0f });
        }
    }



private: // data member
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    FontRender* m_hpUI = nullptr;
    FontRender* m_dashUI = nullptr;
    FontRender* m_knockBackUI = nullptr;

    GameData* m_findGameData = nullptr;

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

};