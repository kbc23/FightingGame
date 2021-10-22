#include "stdafx.h"
#include "player_UI.h"

PlayerUI::PlayerUI()
{
    m_hpUI = NewGO<FontRender>(igo::EnPriority::font);
    m_hpUI->Init(L"1000");

    m_dashUI = NewGO<FontRender>(igo::EnPriority::font);
    m_dashUI->Init(L"3");
}

PlayerUI::~PlayerUI()
{
    DeleteGO(m_hpUI);
    DeleteGO(m_dashUI);
}

bool PlayerUI::Start()
{
    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);

    return true;
}

void PlayerUI::Update()
{
    
}