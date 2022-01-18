#include "stdafx.h"
#include "player_UI.h"

PlayerUI::PlayerUI()
{
    m_hpUI = NewGO<FontRender>(igo::EnPriority::font);
    m_hpUI->Init(L"");

    m_downCountUI = NewGO<FontRender>(igo::EnPriority::font);
    m_downCountUI->Init(L"");
    m_downCountUI->SetPosition({ 100.0f,250.0f });

    //m_mpUI = NewGO<FontRender>(igo::EnPriority::font);
    //m_mpUI->Init(L"4");
    
    m_dashUI = NewGO<FontRender>(igo::EnPriority::font);
    m_dashUI->Init(L"");

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
    m_defenseValueUI->Init(L"");
}

PlayerUI::~PlayerUI()
{
    DeleteGO(m_hpUI);
    DeleteGO(m_dashUI);
    DeleteGO(m_knockBackUI);
    DeleteGO(m_downUI);
    DeleteGO(m_defenseUI);
    DeleteGO(m_downCountUI);
}

bool PlayerUI::Start()
{
    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);

    return true;
}

void PlayerUI::Update()
{
    
}