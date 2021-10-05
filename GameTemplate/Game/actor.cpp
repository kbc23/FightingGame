#include "stdafx.h"
#include "actor.h"



namespace
{
    const enum PlayerNum
    {
        enPlayer_1, //プレイヤー１
        enPlayer_2  //プレイヤー２
    };
}



Actor::Actor()
{
    m_modelCharacter = NewGO<ModelRender>(0);
    m_modelCharacter->Init("Assets/modelData/unityChan.tkm");
}

Actor::~Actor()
{

}

bool Actor::Start()
{
    return true;
}

void Actor::Init(
    GamePad& gamePad,
    const char* filePath,
    const Vector3& initPos,
    float initRotAngle,
    Actor* pOtherActor
)
{
    m_gamePad = &gamePad;



    m_modelCharacter = NewGO<ModelRender>(0);
    m_modelCharacter->Init(filePath);
    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    m_otherActor = pOtherActor;
}

void Actor::Update()
{
    if (m_gamePad->GetLStickXF() != 0.0f) {
        m_position.x -= m_gamePad->GetLStickXF() * 5.0f;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        m_position.z -= m_gamePad->GetLStickYF() * 5.0f;
    }

    if (m_gamePad->IsPress(enButtonA) == true) {
        m_rotY += 0.01f;
        m_rotation.SetRotationY(m_rotY);
    }
    if (m_gamePad->IsPress(enButtonB) == true) {
        m_rotY -= 0.01f;
        m_rotation.SetRotationY(m_rotY);
    }

    m_modelCharacter->SetPosition(m_position);
    //m_shadowModelCharacter->SetPosition(m_position);

    m_modelCharacter->SetRotation(m_rotation);
    //m_shadowModelCharacter->SetRotation(m_rotation);
}

void Actor::Controller()
{

}