#pragma once
#include "font_render.h"
#include "game_data.h"

/**
 * @brief �v���C���[�Ɋւ���UI���Ǘ�����N���X
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

    void SetPosition(const int playerNum)
    {
        if (playerNum == m_findGameData->GetPlayerNum()) {
            m_hpUI->SetPositionX(-200.0f);
        }
        else if (playerNum == m_findGameData->GetOtherPlayerNum()) {
            m_hpUI->SetPositionX(200.0f);
        }
    }



private: // data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    FontRender* m_hpUI = nullptr;

    GameData* m_findGameData = nullptr;

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

};