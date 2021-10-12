#include "stdafx.h"
#include "my_debug.h"

#include "constant.h"
#include "actor.h"
#include "player.h"
#include "player_camera.h"

//MyDebug* MyDebug::m_instance = nullptr;

void MyDebug::Init()
{
    const Vector3 pos[] = {
    {100.0f, 0.0f, 0.0f},		// 1PÇÃèâä˙ç¿ïW
    {-100.0f, 0.0f, 0.0f},		// 2PÇÃèâä˙ç¿ïW
    };
    float rotAngle[] = {
    -90.0f,
    0.0f
    };

    m_player[con::enPlayer] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);
    m_player[con::enPlayer]->DebugInit("Assets/modelData/unityChan.tkm", con::enPlayer,
        pos[con::enPlayer], rotAngle[con::enPlayer]);
    m_player[con::enOtherPlayer] = NewGO<Player>(igo::EnPriority::normal, igo::className::OTHER_PLAYER);
    m_player[con::enOtherPlayer]->DebugInit("Assets/modelData/unityChan.tkm", con::enOtherPlayer,
        pos[con::enOtherPlayer], rotAngle[con::enOtherPlayer]);

    m_playerCamera = NewGO<PlayerCamera>(igo::EnPriority::normal, igo::className::PLAYER_CAMERA);

    //m_actor[con::enPlayer] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    //m_actor[con::enPlayer]->DebugInit("Assets/modelData/unityChan.tkm", con::enPlayer, pos[0]);
    //m_actor[con::enOtherPlayer] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    //m_actor[con::enOtherPlayer]->DebugInit("Assets/modelData/unityChan.tkm", con::enOtherPlayer, pos[1]);
}

void MyDebug::Finish()
{

}

bool MyDebug::Start()
{
    return true;
}

void MyDebug::Update()
{
    m_playerCamera->SetPlayerPosition(m_player[con::enPlayer]->GetPosition());
    m_playerCamera->SetEnemyPosition(m_player[con::enOtherPlayer]->GetPosition());
}

void MyDebug::SoloMode()
{

}