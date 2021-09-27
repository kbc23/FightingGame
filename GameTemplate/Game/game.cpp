#include "stdafx.h"
#include "game.h"

Game::Game()
{
    m_modelCharacter = NewGO<ModelRender>(0);
    m_modelCharacter->Init("Assets/modelData/unityChan.tkm");
    m_shadowModelCharacter = NewGO<Shadow>(0);
    m_shadowModelCharacter->Init("Assets/modelData/unityChan.tkm");

    //m_modelCharacter->SetPosition({ 0.0f,50.0f,0.0f });
    //m_shadowModelCharacter->SetPosition({ 0.0f,50.0f,0.0f });


    m_model = NewGO<ModelRender>(0);
    m_model->Init("Assets/modelData/bg/bg.tkm", true);

    //g_camera3D->SetTarget({ 0.0f,0.0f,0.0f });
    //g_camera3D->SetPosition({ 0.0f,800.0f,300.0f });
}

Game::~Game()
{
    DeleteGO(m_modelCharacter);
    DeleteGO(m_model);
}

bool Game::Start()
{
    return true;
}

void Game::Update()
{
    if (g_pad[0]->IsPress(enButtonA)) {
        m_position.x += 5.0f;
    }
    if (g_pad[0]->IsPress(enButtonB)) {
        m_position.x -= 5.0f;
    }
    if (g_pad[0]->IsPress(enButtonX)) {
        m_position.z += 5.0f;
    }
    if (g_pad[0]->IsPress(enButtonY)) {
        m_position.z -= 5.0f;
    }

    m_modelCharacter->SetPosition(m_position);
    m_shadowModelCharacter->SetPosition(m_position);
}