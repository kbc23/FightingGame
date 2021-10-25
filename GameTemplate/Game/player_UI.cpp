#include "stdafx.h"
#include "player_UI.h"

PlayerUI::PlayerUI()
{
    m_hpUI = NewGO<FontRender>(igo::EnPriority::font);
    m_hpUI->Init(L"1000");

    m_dashUI = NewGO<FontRender>(igo::EnPriority::font);
    m_dashUI->Init(L"3");

    m_knockBackUI = NewGO<FontRender>(igo::EnPriority::font);
    m_knockBackUI->Init(L"ノックバック！");
    m_knockBackUI->Deactivate();
}

PlayerUI::~PlayerUI()
{
    DeleteGO(m_hpUI);
    DeleteGO(m_dashUI);
    DeleteGO(m_knockBackUI);
}

bool PlayerUI::Start()
{
    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);

    return true;
}

void PlayerUI::Update()
{
    
}