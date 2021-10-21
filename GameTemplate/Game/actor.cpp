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
    const char* filePath,
    const Vector3& initPos,
    float initRotAngle,
    Actor* pOtherActor
)
{
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

void Actor::AddStatus(Vector3& addMoveAmount)
{
    // �L�����N�^�[�̈ʒu������
    m_position = m_charaCon.Execute(addMoveAmount, 1.0f);
    // �L�����N�^�[�̌���������
    Turn(addMoveAmount);
}

void Actor::Turn(Vector3& addMoveAmount)
{
    if (fabsf(addMoveAmount.x) < 0.001f
        && fabsf(addMoveAmount.z) < 0.001f) {
        // m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
        // ���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ�
        return;
    }
    // atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐�
    // m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�
    // atan2���g�p���āA�p�x�����߂Ă���
    // ���ꂪ��]�p�x�ɂȂ�
    float angle = atan2(addMoveAmount.x, addMoveAmount.z);
    // atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
    // SetRotationDeg�ł͂Ȃ�SetRotation���g�p����
    m_rotation.SetRotation(Vector3::AxisY, angle);

}

void Actor::SetModelStatus()
{
    // �ʒu�E��]�E�g�����n��
    m_modelCharacter->SetPosition(m_position);
    m_modelCharacter->SetRotation(m_rotation);
    m_modelCharacter->SetScale(m_scale);
}