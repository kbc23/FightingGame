#include "stdafx.h"
#include "player_UI.h"

PlayerUI::PlayerUI()
{
    m_hpUI = NewGO<FontRender>(igo::EnPriority::font);
    m_hpUI->Init(L"1000");
}

PlayerUI::~PlayerUI()
{

}

bool PlayerUI::Start()
{
    return true;
}

void PlayerUI::Update()
{

}