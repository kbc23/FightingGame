#include "stdafx.h"
#include "my_debug.h"

#include "constant.h"
#include "player.h"
#include "player_camera.h"
#include "game_data.h"

namespace
{
    enum EnPlayer
    {
        enPlayer,
        enOtherPlayer
    };
}

void MyDebug::Init()
{
    m_player[enPlayer] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);
    m_player[enOtherPlayer] = NewGO<Player>(igo::EnPriority::normal, igo::className::OTHER_PLAYER);

    m_playerCamera = NewGO<PlayerCamera>(igo::EnPriority::normal, igo::className::PLAYER_CAMERA);

    const Vector3 pos[] = {
    {100.0f, 0.0f, 0.0f},		// 1PÇÃèâä˙ç¿ïW
    {-100.0f, 0.0f, 0.0f},		// 2PÇÃèâä˙ç¿ïW
    };
    float rotAngle[] = {
    -90.0f,
    0.0f
    };

    m_gameData = NewGO<GameData>(igo::EnPriority::normal, igo::className::GAME_DATA);
    m_gameData->SetPlayerNumAndOtherPlayerNum(enPlayer, enOtherPlayer);

    m_playerCamera->SetPlayerNum(enPlayer, enOtherPlayer);

    m_player[enPlayer]->DebugInit("Assets/modelData/unityChan.tkm", enPlayer,
        pos[enPlayer], rotAngle[enPlayer], m_player[enOtherPlayer]);
    m_player[enOtherPlayer]->DebugInit("Assets/modelData/unityChan.tkm", enOtherPlayer,
        pos[enOtherPlayer], rotAngle[enOtherPlayer], m_player[enPlayer]);

    m_modelStage = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelStage->Init("Assets/modelData/bg/bg.tkm", true);

    m_modelStage->SetPosition({ 0.0f,0.0f,0.0f });
}

void MyDebug::Finish()
{
    DeleteGO(m_modelStage);
}

bool MyDebug::Start()
{
    return true;
}

void MyDebug::Update()
{
    m_playerCamera->SetPlayerPosition(m_player[enPlayer]->GetPosition());
    m_playerCamera->SetEnemyPosition(m_player[enOtherPlayer]->GetPosition());
}

void MyDebug::SoloMode()
{

}