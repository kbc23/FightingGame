#include "stdafx.h"
#include "player.h"

#include "constant.h"
#include "player_camera.h"
#include "player_UI.h"
#include "game_data.h"
#include "player_controller.h"

Player::Player()
{

}

Player::~Player()
{
    DeleteGO(m_actor);
    DeleteGO(m_playerUI);
}

bool Player::Start()
{
    return true;
}

void Player::Init(
    GamePad& gamePad,
    const Vector3& initPos,
    float initRotAngle,
    int playerNum,
    Player* pOtherPlayer
)
{
    // �����Ŏ����Ă����������̃N���X�ŕێ�
    m_gamePad = &gamePad;
    m_playerNum = playerNum;
    m_otherPlayer = pOtherPlayer;

    //////////////////////////////
    // NewGO
    //////////////////////////////

    // Actor�N���X�̏�����
    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->Init(initPos, initRotAngle);

    // �����蔻��̏�����
    m_hitbox = NewGO<Hitbox>(igo::EnPriority::normal);
    m_hitbox->Init(*m_otherPlayer, *m_actor, m_playerStatus);

    // �v���C���[�Ɋ֌W����UI�N���X�̏�����
    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    m_playerController = NewGO<PlayerController>(igo::EnPriority::normal);
    m_playerController->Init(*m_gamePad, m_playerStatus);

    //////////////////////////////
    // FindGO
    //////////////////////////////

    // �v���C���[�̃J����
    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);
    // �Q�[���f�[�^
    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);
}

void Player::Update()
{
    // ����
    Controller();

    // �U���֘A�̖��t���[���̏���
    UpdateAttack();

    //////////////////////////////
    // UI��Update
    //////////////////////////////

    m_playerUI->UpdateHpUI(m_hp, m_playerNum);
    //m_playerUI->UpdateDashUI(m_dashStatus.GetRemainingNumberOfTimes(), m_playerNum);
    //m_playerUI->UpdateKnockBackUI(m_squeezeStatus.GetFlagSqueeze(), m_playerNum);
    //m_playerUI->UpdateDownUI(m_downStatus.GetFlagDown(), m_playerNum);
    //m_playerUI->UpdateDefenseUI(m_defenceData.GetFlagDefense(), m_playerNum);
}

////////////////////////////////////////////////////////////
// �ړ�����
////////////////////////////////////////////////////////////

void Player::Controller()
{
    // �Q�[���I�����A���������Ȃ�
    if (true == m_flagGameEndStopOperation) {
        return;
    }

    if (true == m_playerStatus.NotController()) {
        return;
    }

    //////////////////////////////
    // �{�^������
    //////////////////////////////

    //m_playerController->ControllerButton();

    // A�{�^��
    if (false == m_playerStatus.NotAttack() && true == m_gamePad->IsTrigger(enButtonA)) {
        m_playerStatus.SetAttackData(enButtonA);
    }
    // B�{�^��
    if (false == m_playerStatus.NotAttack() && true == m_gamePad->IsTrigger(enButtonB)) {
        m_playerStatus.SetAttackData(enButtonB);
    }
    // X�{�^��
    if (false == m_playerStatus.NotAttack() && true == m_gamePad->IsTrigger(enButtonX)) {
        m_playerStatus.SetAttackData(enButtonX);
    }
    // Y�{�^��
    if (false == m_playerStatus.NotAttack() && true == m_gamePad->IsTrigger(enButtonY)) {
        m_playerStatus.SetAttackData(enButtonY);
    }
    // L1�{�^��
    if (false == m_playerStatus.CheckNowDefence() && true == m_gamePad->IsTrigger(enButtonLB1)) {
        m_playerStatus.SetAttackData(enButtonLB1);
    }
    // L2�{�^��
    if (false == m_playerStatus.NotAttack() && true == m_gamePad->IsTrigger(enButtonLB2)) {
        m_playerStatus.SetAttackData(enButtonLB2);
    }
    // R1�{�^��
    if (false == m_playerStatus.CheckNowDefence() && true == m_gamePad->IsTrigger(enButtonRB1)) {
        m_playerStatus.SetAttackData(enButtonRB1);
    }
    // R2�{�^��
    if (false == m_playerStatus.NotAttack() && true == m_gamePad->IsTrigger(enButtonRB2)) {
        m_playerStatus.SetAttackData(enButtonRB2);
    }

    // R1�{�^��: �_�b�V��
    //if (false == m_getStPlayerStatus->CheckNowDefence() && true == m_gamePad->IsTrigger(enButtonRB1)) {
    //    m_getStPlayerStatus->StartDash();
    //}
    // L1�{�^��: �K�[�h
    if (false == m_playerStatus.CheckNowDash() && true == m_gamePad->IsPress(enButtonA)) {
        m_playerStatus.StartDefence();
        m_actor->SetDefenceAnimation();
    }

    // Debug: start
    //if (m_playerNum == m_findGameData->GetOtherPlayerNum()) {
    //    m_defenceData.SetFlagDefense(true);
    //}
    // Debug: end

    // Debug: Start�{�^��: �Q�[���I��
    if (true == m_gamePad->IsTrigger(enButtonStart)) {
        MessageBoxA(nullptr, "�Q�[�����I�����܂�(Debug End)", "���b�Z�[�W", MB_OK);
        //�Q�[�����I��
        exit(EXIT_SUCCESS);
    }
    // Debug: end


    // �v���C���[�̈ړ�
    Vector3 moveAmount = m_playerController->ControllerLStick();

    // �K�[�h���͏��������Ȃ�
    //if (false == m_playerStatus.CheckNowDefence()) {
        //if (false == m_playerStatus.CheckNowDash()) {
    moveAmount = Move(moveAmount);
    //}
    //// �_�b�V��
    //else {
    //    moveAmount = DashMove();
    //}
//}
//else {
//    moveAmount = Vector3::Zero;
//}

    Defence(moveAmount.y);

    // �X�E�F�[�̏���
    Vector2 swayMove = m_playerController->ControllerRStick();

    // �U�����̃A�j���[�V�������Đ�
    if (true == m_playerStatus.CheckNowAttack()) {
        AttackAnimationStart();
    }
    // �v���C���[�̃��f���Ɉʒu���Ȃǂ̃X�e�[�^�X����n��
    m_actor->AddStatus(moveAmount, swayMove);
}

const Vector3 Player::Move(const Vector3& moveAmountBeforeEditing)
{
    // �J�����̑O�������擾
    Vector3 cameraFront = m_actor->GetPosition() - m_findPlayerCamera->GetPosition(m_playerNum);
    cameraFront.y = 0.0f;
    cameraFront.Normalize();

    // �J�����̉E����
    Vector3 cameraRight = Cross(g_vec3AxisY, cameraFront);

    // �L�����N�^�[�̈ړ����x
    const float CHARACTER_SPEED = 9.0f;

    Vector3 pospos = moveAmountBeforeEditing * CHARACTER_SPEED;

    //�L�����N�^�[�̈ړ��ʂ��v�Z
    Vector3 moveAmount = cameraFront * pospos.z + cameraRight * pospos.x;

    return moveAmount;
}

const Vector3 Player::DashMove()
{
    // �_�b�V���̂��߂ɉ��ō쐬����|�W�V����
    Vector3 attackRangePosition =
    {
        m_actor->GetPosition().x,
        m_actor->GetPosition().y,
        m_actor->GetPosition().z - 1.0f // �J�����̑O�����ɏ����������ɒu��
    };
    // �L�����N�^�[�̃|�W�V����
    Vector3 playerPosition = m_actor->GetPosition();

    // ���̃|�W�V��������L�����N�^�[�̃|�W�V�����̃x�N�g�����擾
    Vector3 toPos = playerPosition - attackRangePosition;

    // �L�����N�^�[�̈ړ����x
    float characterSpeed = 80.0f;

    // �L�����N�^�[�̈ړ��ʂ̌v�Z
    toPos.z *= characterSpeed;
    // �L�����N�^�[��Quaternion���g���ăx�N�g�����v���C���[�̑O�����ɉ�]������
    m_actor->GetRotation().Apply(toPos);

    // ��L�Ŏ擾������񂩂�A�U���͈͂𐶐�����|�W�V�������擾
    return toPos;
}

void Player::Defence(const float moveY)
{
    // �������ɓ���
    if (-0.5f >= moveY) {
        // �h��̏���
        m_playerStatus.StartDefence();
    }
    // �������ɓ��͂��Ă��Ȃ�
    else if (-0.5f < moveY) {
        // �N���オ��
        m_playerStatus.EndDefence();
    }
}

////////////////////////////////////////////////////////////
// �U���֘A
////////////////////////////////////////////////////////////

void Player::UpdateAttack()
{
    // �v���C���[�̃X�e�[�^�X�֌W�̍X�V����
    m_playerStatus.StatusUpdate();

    // �U�������������Ƃ��̏���
    if (true == m_hitbox->UpdateCheckAttack()) {
        // �U������������
        HitAttack();
    }
}

void Player::HitAttack()
{    
    // �_���[�W����
    if (false == m_otherPlayer->ReceiveDamage(m_playerStatus.GetAttackPower())) {
        // �_���[�W��^�����ĂȂ�
        return;
    }
}

void Player::AttackAnimationStart()
{
    // �U�����̃A�j���[�V�����̍Đ����J�n����֐�
    // ���̊֐��́A�ǂ������̏����ɕς�����
    // �iActor�N���X�̃C���X�^���X�ŏ���������悤�ɂ������j

    if (StAttackData::EnAttackType::enJubHead_L == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enJubHead_L);
        return;
    }
    if (StAttackData::EnAttackType::enJubHead_R == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enJubHead_R);
        return;
    }
    if (StAttackData::EnAttackType::enJubBody_L == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enJubBody_L);
        return;
    }
    if (StAttackData::EnAttackType::enJubBody_R == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enJubBody_R);
        return;
    }
    if (StAttackData::EnAttackType::enUppercut == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enUppercut);
        return;
    }
    if (StAttackData::EnAttackType::enHook == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enHook);
        return;
    }
    if (StAttackData::EnAttackType::enBodyBlow == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enBodyBlow);
        return;
    }
    if (StAttackData::EnAttackType::enStraight == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enStraight);
        return;
    }
}