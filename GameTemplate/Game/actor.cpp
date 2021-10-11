#include "stdafx.h"
#include "actor.h"



namespace
{

}



Actor::Actor()
{

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
    //m_gamePad = &gamePad;

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true);
    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    m_otherActor = pOtherActor;
}

void Actor::DebugInit(const char* filePath, const int playerNum, const Vector3& initPos, const float initRot)
{
    //m_gamePad = g_pad[playerNum];

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true);

    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    m_rotY = initRot;
    m_rotation.SetRotationY(m_rotY);
    m_modelCharacter->SetRotation(m_rotation);
}

void Actor::Update()
{
    //if (m_gamePad->GetLStickXF() != 0.0f) {
    //    m_position.x -= m_gamePad->GetLStickXF() * 5.0f;
    //}
    //if (m_gamePad->GetLStickYF() != 0.0f) {
    //    m_position.z -= m_gamePad->GetLStickYF() * 5.0f;
    //}

    //if (m_gamePad->IsPress(enButtonA) == true) {
    //    m_rotY += 0.01f;
    //    m_rotation.SetRotationY(m_rotY);
    //}
    //if (m_gamePad->IsPress(enButtonB) == true) {
    //    m_rotY -= 0.01f;
    //    m_rotation.SetRotationY(m_rotY);
    //}

    SetModelStatus();
}

void Actor::AddStatus(const Vector3& addPos, const float addRotAngle)
{
    m_position += addPos;
    m_rotY += addRotAngle;
    m_rotation.SetRotationY(m_rotY);
}

void Actor::SetModelStatus()
{
    // ˆÊ’uE‰ñ“]EŠg‘åî•ñ‚ð“n‚·
    m_modelCharacter->SetPosition(m_position);
    m_modelCharacter->SetRotation(m_rotation);
    m_modelCharacter->SetScale(m_scale);
}