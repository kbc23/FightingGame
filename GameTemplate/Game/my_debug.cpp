#include "stdafx.h"
#include "my_debug.h"

#include "constant.h"
#include "actor.h"

//MyDebug* MyDebug::m_instance = nullptr;

void MyDebug::Init()
{
    const Vector3 pos[] = {
    {100.0f, 0.0f, 0.0f},		// 1P‚Ì‰ŠúÀ•W
    {-100.0f, 0.0f, 0.0f},		// 2P‚Ì‰ŠúÀ•W
    };

    m_actor[con::enPlayer_1] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor[con::enPlayer_1]->DebugInit("Assets/modelData/unityChan.tkm", con::enPlayer_1, pos[0]);
    m_actor[con::enPlayer_2] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor[con::enPlayer_2]->DebugInit("Assets/modelData/unityChan.tkm", con::enPlayer_2, pos[1]);
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