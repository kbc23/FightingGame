#include "stdafx.h"
#include "actor.h"



namespace // constant
{
    const float PLAYER_COLLIDER_RADIUS = 20.0f;
    const float PLAYER_COLLIDER_HEIGHT = 100.0f;

}



Actor::Actor()
{
    m_readCSVFileCharacterData.ReadCSVFileData("Assets/Data/data.csv");
}

Actor::~Actor()
{

}

bool Actor::Start()
{
    return true;
}

////////////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////////////

void Actor::Init(const Vector3& initPos, float initRotAngle)
{
    //�A�j���[�V�����̐ݒ�
    if (false == AnimationInit()) {
        MessageBoxA(nullptr, "�L�����N�^�[�̃A�j���[�V�����̏������Ɏ��s�����̂ŏI�����܂�", "�G���[", MB_OK);
        //�Q�[�����I��
        exit(EXIT_SUCCESS);
    }

    // �L�����N�^�[���f��
    CharacterInit(initPos, initRotAngle);
}

const bool Actor::AnimationInit()
{
    for (int num = 0; AnimationEnum::AnimationMax > num; ++num) {
        if (false == m_animationPlayer[num].Load(
            m_readCSVFileCharacterData.GetCharacterAnimationPath(num).c_str())
            )
        {
            return false;
        }
    }

    //���[�v�Đ���true�ɂ���
    m_animationPlayer[AnimationEnum::enIdle].SetLoopFlag(true);

    return true;
}

void Actor::CharacterInit(const Vector3& initPos, const float initRot)
{
    // �L�����N�^�[���f��
    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(m_readCSVFileCharacterData.GetCharacterModelPath().c_str(),
        false, true, modelUpAxis::enModelUpAxisZ,
        m_animationPlayer, AnimationEnum::AnimationMax
    );

    //�A�j���[�V�����̍Đ�
    m_modelCharacter->PlayAnimation(AnimationEnum::enIdle);

    // �����ʒu�̐ݒ�
    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    // ������]�̐ݒ�
    m_rotY = initRot;
    m_rotation.SetRotationY(m_rotY);
    m_modelCharacter->SetRotation(m_rotation);
    // �����g�嗦�̐ݒ�
    m_scale = { 2.0f,2.0f,2.0f };
    m_modelCharacter->SetScale(m_scale);
}

void Actor::Update()
{
    KnockDownAnimation();
    // �U���A�j���[�V�����֘A
    AttackAnimation();
    // ���f���Ɉʒu���Ȃǂ̃f�[�^��n��
    SetModelStatus();
}

void Actor::AddStatus(Vector3& addMoveAmount, const Vector2& swayMove)
{
    // �L�����N�^�[�̈ʒu������
    m_position += addMoveAmount;
    // �L�����N�^�[�̌���������
    PlayerViewingDirection();

    // �X�E�F�[�̃Z�b�g
    m_modelCharacter->SetSwayMove(swayMove);

    // ���Ⴊ�݂̏���
    CrouchingUpdate(swayMove.y);
}

void Actor::PlayerViewingDirection()
{
    if (m_getOtherActor == nullptr) {
        return;
    }

    Vector3 viewingDirection = m_getOtherActor->GetPosition() - m_position;

    // atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐�
    // m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�
    // atan2���g�p���āA�p�x�����߂Ă���
    // ���ꂪ��]�p�x�ɂȂ�
    float viewingAngle = atan2(viewingDirection.x, viewingDirection.z);
    // atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
    // SetRotationDeg�ł͂Ȃ�SetRotation���g�p����
    m_rotation.SetRotation(Vector3::AxisY, viewingAngle);
}

void Actor::AttackAnimation()
{
    if (true == m_hp0) {
        return;
    }

    // �U���̃A�j���[�V�������ł͂Ȃ��ꍇ
    if (false == m_flagAttackAnimation) {
        return;
    }

    // �A�j���[�V�������Đ�����Ă��Ȃ��i�U���̃A�j���[�V�����̍Đ����I������j
    if (false == m_modelCharacter->IsPlayingAnimation()) {
        m_modelCharacter->PlayAnimation(AnimationEnum::enIdle); // �ʏ펞�̃A�j���[�V�����ɂ���
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

void Actor::CrouchingUpdate(const float swayMoveY)
{
    // �O�����ɓ���
    if (0.5f <= swayMoveY) {
        // ���Ⴊ��
        CrouchingStart();
    }
    // �O�����ɓ��͂��Ă��Ȃ�
    else if (0.5f > swayMoveY) {
        // �N���オ��
        CrouchingEnd();
    }

    // ���Ⴊ�ݒ��̏���
    Crouching();
}

void Actor::CrouchingStart()
{
    // ���Ⴊ��
    if (EnCrouchingStatus::enNotCrouching == m_crouchingStatus) {
        m_crouchingStatus = EnCrouchingStatus::enStart;
        m_modelCharacter->PlayAnimation(AnimationEnum::enCrouchingStart);
    }
}

void Actor::Crouching()
{
    // ���Ⴊ�݊J�n
    if (EnCrouchingStatus::enStart == m_crouchingStatus) {
        // �A�j���[�V�������I���
        if (false == m_modelCharacter->IsPlayingAnimation()) {
            // ���Ⴊ�ݒ��̏�Ԃɂ��A�A�j���[�V�������Đ�
            m_crouchingStatus = EnCrouchingStatus::enCrouching;
            m_modelCharacter->PlayAnimation(AnimationEnum::enCrouching);
        }
    }
    // ���Ⴊ�ݒ�
    else if (EnCrouchingStatus::enCrouching == m_crouchingStatus) {
        // �A�j���[�V�������I���
        if (false == m_modelCharacter->IsPlayingAnimation()) {
            // �N���オ���Ԃɂ��A�A�j���[�V�������Đ�
            m_crouchingStatus = EnCrouchingStatus::enEnd;
            m_modelCharacter->PlayAnimation(AnimationEnum::enCrouchingEnd);
        }
    }
    // �N���オ�蒆
    else if (EnCrouchingStatus::enEnd == m_crouchingStatus) {
        // �A�j���[�V�������I���
        if (false == m_modelCharacter->IsPlayingAnimation()) {
            // �ʏ��Ԃɂ��A�A�j���[�V�������Đ�
            m_crouchingStatus = EnCrouchingStatus::enNotCrouching;
            m_modelCharacter->PlayAnimation(AnimationEnum::enIdle);
        }
    }
}

void Actor::CrouchingEnd()
{
    // ���Ⴊ�ݒ�
    if (EnCrouchingStatus::enCrouching == m_crouchingStatus) {
        // �N���オ���Ԃɂ��A�A�j���[�V�������Đ�
        m_crouchingStatus = EnCrouchingStatus::enEnd;
        m_modelCharacter->PlayAnimation(AnimationEnum::enCrouchingEnd);
    }
}