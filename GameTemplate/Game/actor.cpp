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
    float initRotAngle
)
{
    //�A�j���[�V�����̐ݒ�
    m_animationPlayer[idle].Load("Assets/modelData/model/idle.tka");
    m_animationPlayer[jub].Load("Assets/modelData/model/Lead_Jab.tka");
    m_animationPlayer[uppercut].Load("Assets/modelData/model/Uppercut.tka");
    m_animationPlayer[hook].Load("Assets/modelData/model/hook.tka");
    m_animationPlayer[bodyBlow].Load("Assets/modelData/model/Body_blow.tka");
    m_animationPlayer[straight].Load("Assets/modelData/model/straight.tka");
    //���[�v�Đ���true�ɂ���
    m_animationPlayer[idle].SetLoopFlag(true);

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true, modelUpAxis::enModelUpAxisZ, m_animationPlayer, AnimationMax);
    m_modelCharacter->PlayAnimation(idle); //�A�j���[�V�����̍Đ�

    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    m_rotY = initRotAngle;
    m_rotation.SetRotationY(m_rotY);
    m_modelCharacter->SetRotation(m_rotation);

    m_scale = { 2.0f,2.0f,2.0f };
    m_modelCharacter->SetScale(m_scale);
}

void Actor::DebugInit(const char* filePath, const Vector3& initPos, const float initRot)
{
    //�A�j���[�V�����̐ݒ�
    m_animationPlayer[idle].Load("Assets/modelData/model/idle.tka");
    m_animationPlayer[jub].Load("Assets/modelData/model/Lead_Jab.tka");
    m_animationPlayer[uppercut].Load("Assets/modelData/model/Uppercut.tka");
    m_animationPlayer[hook].Load("Assets/modelData/model/hook.tka");
    m_animationPlayer[bodyBlow].Load("Assets/modelData/model/Body_blow.tka");
    m_animationPlayer[straight].Load("Assets/modelData/model/straight.tka");
    //���[�v�Đ���true�ɂ���
    m_animationPlayer[idle].SetLoopFlag(true);

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true, modelUpAxis::enModelUpAxisZ, m_animationPlayer, AnimationMax);
    m_modelCharacter->PlayAnimation(idle); //�A�j���[�V�����̍Đ�
    //m_modelCharacter->Init(filePath, false, true, modelUpAxis::enModelUpAxisZ);

    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    m_rotY = initRot;
    m_rotation.SetRotationY(m_rotY);
    m_modelCharacter->SetRotation(m_rotation);

    m_scale = { 2.0f,2.0f,2.0f };
    m_modelCharacter->SetScale(m_scale);
}

void Actor::Update()
{
    AttackAnimation();

    SetModelStatus();
}

void Actor::AddStatus(Vector3& addMoveAmount)
{
    // �L�����N�^�[�̈ʒu������
    m_position += addMoveAmount;
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

void Actor::AttackAnimation()
{
    // �U���̃A�j���[�V�������ł͂Ȃ��ꍇ
    if (false == m_flagAttackAnimation) {
        return;
    }

    // �A�j���[�V�������Đ�����Ă��Ȃ��i�U���̃A�j���[�V�����̍Đ����I������j
    if (false == m_modelCharacter->IsPlayingAnimation()) {
        m_modelCharacter->PlayAnimation(idle);
        m_flagAttackAnimation = false;
    }
}

void Actor::SetModelStatus()
{
    // �ʒu�E��]�E�g�����n��
    m_modelCharacter->SetPosition(m_position);
    m_modelCharacter->SetRotation(m_rotation);
    m_modelCharacter->SetScale(m_scale);
}