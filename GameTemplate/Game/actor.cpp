#include "stdafx.h"
#include "actor.h"



namespace
{
    const float PLAYER_COLLIDER_RADIUS = 20.0f;
    const float PLAYER_COLLIDER_HEIGHT = 100.0f;

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

    //�L�����R���̏�����
    m_charaCon.Init(PLAYER_COLLIDER_RADIUS, PLAYER_COLLIDER_HEIGHT, m_position);
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

    //�L�����R���̏�����
    m_charaCon.Init(PLAYER_COLLIDER_RADIUS, PLAYER_COLLIDER_HEIGHT, m_position);
}

void Actor::Update()
{
    SetModelStatus();
}

void Actor::AddStatus(Vector3& addMoveAmount, const float addRotAngle)
{
    m_position = m_charaCon.Execute(addMoveAmount, 1.0f); // �L�����N�^�[�̈ʒu������
    m_rotY += addRotAngle;
    m_rotation.SetRotationY(m_rotY); // �L�����N�^�[�̌���������
}

void Actor::SetModelStatus()
{
    // �ʒu�E��]�E�g�����n��
    m_modelCharacter->SetPosition(m_position);
    m_modelCharacter->SetRotation(m_rotation);
    m_modelCharacter->SetScale(m_scale);
}