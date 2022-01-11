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

    void UpdateDownUI(const bool down, const int playerNum)
    {
        if (true == down) {
            m_downUI->Activate();
        }
        else {
            m_downUI->Deactivate();
        }

        SetPosition(playerNum);
    }

    void UpdateDefenseUI(const bool defense, const int playerNum)
    {
        if (true == defense) {
            m_defenseUI->Activate();
        }
        else {
            m_defenseUI->Deactivate();
        }

        SetPosition(playerNum);
    }

    void UpdateDefenseValueUI(const int defenseValue, const int playerNum)
    {
        m_defenseValueUI->SetText(defenseValue);
        SetPosition(playerNum);
    }

    void UpdateDownCountUI(const int time)
    {
        int textValue = time / 60;
        m_downCountUI->SetText(textValue);
    }

    void ResetDownCountUI()
    {
        m_downCountUI->SetText(L"");
    }

    void SetPosition(const int playerNum)
    {
        if (playerNum == m_findGameData->GetPlayerNum()) {
            m_hpUI->SetPositionX(-200.0f);
            m_dashUI->SetPosition({ -200.0f,-100.0f });
            m_knockBackUI->SetPosition({ -200.0f,-150.0f });
            m_downUI->SetPosition({ -200.0f,-200.0f });
            m_defenseUI->SetPosition({ -200.0f,-250.0f });
            m_defenseValueUI->SetPosition({ -200.0f,-300.0f });
        }
        else if (playerNum == m_findGameData->GetOtherPlayerNum()) {
            m_hpUI->SetPositionX(200.0f);
            m_dashUI->SetPosition({ 200.0f,-100.0f });
            m_knockBackUI->SetPosition({ 200.0f,-150.0f });
            m_downUI->SetPosition({ 200.0f,-200.0f });
            m_defenseUI->SetPosition({ 200.0f,-250.0f });
            m_defenseValueUI->SetPosition({ 200.0f,-300.0f });
        }
    }



private: // data member
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    FontRender* m_hpUI = nullptr;
    FontRender* m_dashUI = nullptr;
    FontRender* m_knockBackUI = nullptr;
    FontRender* m_downUI = nullptr;
    FontRender* m_defenseUI = nullptr;
    FontRender* m_defenseValueUI = nullptr;

    FontRender* m_downCountUI = nullptr;

    GameData* m_findGameData = nullptr;

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

};