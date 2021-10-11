#include "stdafx.h"
#include "my_debug.h"

#include "constant.h"
#include "actor.h"
#include "player.h"

//MyDebug* MyDebug::m_instance = nullptr;

void MyDebug::Init()
{
    const Vector3 pos[] = {
    {100.0f, 0.0f, 0.0f},		// 1PÇÃèâä˙ç¿ïW
    {-100.0f, 0.0f, 0.0f},		// 2PÇÃèâä˙ç¿ïW
    };

    m_player[con::enPlayer_1] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);
    m_player[con::enPlayer_1]->DebugInit("Assets/modelData/unityChan.tkm", con::enPlayer_1, pos[con::enPlayer_1]);
    m_player[con::enPlayer_2] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);
    m_player[con::enPlayer_2]->DebugInit("Assets/modelData/unityChan.tkm", con::enPlayer_2, pos[con::enPlayer_2]);


    //m_actor[con::enPlayer_1] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    //m_actor[con::enPlayer_1]->DebugInit("Assets/modelData/unityChan.tkm", con::enPlayer_1, pos[0]);
    //m_actor[con::enPlayer_2] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    //m_actor[con::enPlayer_2]->DebugInit("Assets/modelData/unityChan.tkm", con::enPlayer_2, pos[1]);
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

}

void MyDebug::SoloMode()
{

}