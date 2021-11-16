#include "stdafx.h"
#include "player_UI.h"

PlayerUI::PlayerUI()
{
    m_hpUI = NewGO<FontRender>(igo::EnPriority::font);
    m_hpUI->Init(L"1000");

    //m_mpUI = NewGO<FontRender>(igo::EnPriority::font);
    //m_mpUI->Init(L"4");
    
    m_dashUI = NewGO<FontRender>(igo::EnPriority::font);
    m_dashUI->Init(L"3");

    m_knockBackUI = NewGO<FontRender>(igo::EnPriority::font);
    m_knockBackUI->Init(L"のけぞり");
    m_knockBackUI->Deactivate();

    m_downUI = NewGO<FontRender>(igo::EnPriority::font);
    m_downUI->Init(L"ダウン");
    m_downUI->Deactivate();

    m_defenseUI = NewGO<FontRender>(igo::EnPriority::font);
    m_defenseUI->Init(L"ガード");
    m_defenseUI->Deactivate();

    m_defenseValueUI = NewGO<FontRender>(igo::EnPriority::font);
    m_defenseValueUI->Init(L"100");
}

PlayerUI::~PlayerUI()
{
    DeleteGO(m_hpUI);
    DeleteGO(m_dashUI);
    DeleteGO(m_knockBackUI);
    DeleteGO(m_downUI);
    DeleteGO(m_defenseUI);
}

bool PlayerUI::Start()
{
    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);

    return true;
}

void PlayerUI::Update()
{
    
}