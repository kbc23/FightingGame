#include "stdafx.h"
#include "actor.h"



namespace
{
    const enum PlayerNum
    {
        enPlayer_1, //�v���C���[�P
        enPlayer_2  //�v���C���[�Q
    };
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
    m_gamePad = &gamePad;

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true);
    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    m_otherActor = pOtherActor;
}

void Actor::DebugInit(const char* filePath, const int playerNum, const Vector3& initPos)
{
    m_gamePad = g_pad[playerNum];

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true);

    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
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

    SetModelStatus();
}

void Actor::Controller()
{
    if (false == m_flagOperation) {
        return;
    }

    // �v���C���[�̈ړ�
    if (m_gamePad->GetLStickXF() != 0.0f) {
        m_position.x -= m_gamePad->GetLStickXF() * 5.0f;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        m_position.z -= m_gamePad->GetLStickYF() * 5.0f;
    }



    // A�{�^��: �ʏ�U��
    if (m_gamePad->IsPress(enButtonA) == true) {
        // A�{�^�����������Ƃ��̏���

    }
    // B�{�^��: ����U��
    if (m_gamePad->IsPress(enButtonB) == true) {
        // B�{�^�����������Ƃ��̏���

    }
    //?: �K�E�Z
    if (true) {

    }
    // ?: ���
    if (true) {

    }
    // ?: �K�[�h
    if (true) {

    }
}

void Actor::SetModelStatus()
{
    // �ʒu�E��]�E�g�����n��
    m_modelCharacter->SetPosition(m_position);
    m_modelCharacter->SetRotation(m_rotation);
    m_modelCharacter->SetScale(m_scale);
}