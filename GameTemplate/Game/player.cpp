#include "stdafx.h"
#include "player.h"

#include "constant.h"
#include "player_camera.h"
#include "player_UI.h"
#include "game_data.h"
#include "attack_move_bone.h"

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

    ////////////////////////////////////////////////////////////
    // ������
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    // Actor�N���X�̏�����
    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->Init(
        "Assets/modelData/unityChan.tkm",
        initPos,
        initRotAngle
    );

    // �����蔻��̏�����
    m_hitbox = NewGO<Hitbox>(igo::EnPriority::normal);
    //m_hitbox->Init(*m_otherPlayer, *m_actor, m_attackData, m_defenceData);

    // �v���C���[�Ɋ֌W����UI�N���X�̏�����
    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    //////////////////////////////
    // FindGO
    //////////////////////////////

    // �v���C���[�̃J����
    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);
    // �Q�[���f�[�^
    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);
}

void Player::DebugInit(
    const char* filePath,
    const int playerNum,
    const Vector3& initPos,
    const float initRot,
    Player* pOtherPlayer
)
{
    // �����Ŏ����Ă����������̃N���X�ŕێ�
    m_gamePad = g_pad[playerNum];
    m_playerNum = playerNum;
    m_otherPlayer = pOtherPlayer;

    ////////////////////////////////////////////////////////////
    // ������
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////
    
    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->DebugInit(filePath, initPos, initRot);

    m_hitbox = NewGO<Hitbox>(igo::EnPriority::normal);
    m_hitbox->Init(*m_otherPlayer, *m_actor, m_attackData, m_defenceData);

    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);

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
    m_playerUI->UpdateDashUI(m_dashStatus.GetRemainingNumberOfTimes(), m_playerNum);
    m_playerUI->UpdateKnockBackUI(m_squeezeStatus.GetFlagSqueeze(), m_playerNum);
    m_playerUI->UpdateDownUI(m_downStatus.GetFlagDown(), m_playerNum);
    m_playerUI->UpdateDefenseUI(m_defenceData.GetFlagDefense(), m_playerNum);
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

    // �U�����A���������Ȃ�
    if (true == m_attackData.GetFlagAttackNow()) {
        return;
    }

    // �m�b�N�o�b�N���A���������Ȃ�
    if (true == m_squeezeStatus.GetFlagSqueeze()) {
        return;
    }

    // �_�E�����A���������Ȃ�
    if (true == m_downStatus.GetFlagDown()) {
        return;
    }

    //////////////////////////////
    // �{�^������
    //////////////////////////////

    // A�{�^��
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonA)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enJub)) {
            AttackAnimationStart();
        }
    }
    // B�{�^��
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonB)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enHook)) {
            AttackAnimationStart();
        }
    }
    // X�{�^��
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonX)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enUppercut)) {
            AttackAnimationStart();
        }
    }
    // Y�{�^��
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonY)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enStraight)) {
            AttackAnimationStart();
        }
    }
    // R2�{�^��
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonRB2)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enBodyBlow)) {
            AttackAnimationStart();
        }
    }

    // R1�{�^��: �_�b�V��
    if (false == m_defenceData.GetFlagDefense() && true == m_gamePad->IsTrigger(enButtonRB1)) {
        m_dashStatus.StartDash();
    }

    // L1�{�^��: �K�[�h
    if (false == m_dashStatus.GetFlagDash() && true == m_gamePad->IsPress(enButtonLB1)) {
        m_defenceData.SetFlagDefense(true);
    }
    else {
        m_defenceData.SetFlagDefense(false);
    }

    // Debug: start
    if (m_playerNum == m_findGameData->GetOtherPlayerNum()) {
        m_defenceData.SetFlagDefense(true);
    }
    // Debug: end

    // Debug: Start�{�^��: �Q�[���I��
    if (true == m_gamePad->IsTrigger(enButtonStart)) {
        //�Q�[�����I��
        exit(EXIT_SUCCESS);
    }



    // �v���C���[�̈ړ�
    Vector3 moveAmount = Vector3::Zero;

    // �K�[�h���͏��������Ȃ�
    if (false == m_defenceData.GetFlagDefense()) {
        if (false == m_dashStatus.GetFlagDash()) {
            moveAmount = Move();
        }
        // �_�b�V��
        else {
            moveAmount = DashMove();
        }
    }

    // �X�E�F�[�̏���
    Vector2 swayMove = g_vec2Zero;
    if (m_gamePad->GetRStickXF() != 0.0f) {
        swayMove.x += m_gamePad->GetRStickXF();
    }
    if (m_gamePad->GetRStickYF() != 0.0f) {
        swayMove.y += m_gamePad->GetRStickYF();
    }

    // �v���C���[�̃��f���Ɉʒu���Ȃǂ̃X�e�[�^�X����n��
    m_actor->AddStatus(moveAmount, swayMove);
}

const Vector3 Player::Move()
{
    // ���݂̈ʒu����ۑ�
    //Vector3 oldPos = m_actor->GetPosition();

    // �J�����̑O�������擾
    // ���J�����̑O�������Q�Ƃ���Z���g���Ƃ���[cameraFront]���g���邩��
    Vector3 cameraFront = m_actor->GetPosition() - m_findPlayerCamera->GetPosition(m_playerNum);
    cameraFront.y = 0.0f;
    cameraFront.Normalize();

    // �J�����̉E����
    Vector3 cameraRight = Cross(g_vec3AxisY, cameraFront);

    // �L�����N�^�[�̈ړ����x
    float characterSpeed = 9.0f;

    Vector3 pospos = Vector3::Zero;

    if (m_gamePad->GetLStickXF() != 0.0f) {
        pospos.x += m_gamePad->GetLStickXF() * characterSpeed;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        pospos.z += m_gamePad->GetLStickYF() * characterSpeed;
    }

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
    toPos.z = toPos.z * characterSpeed;
    // �L�����N�^�[��Quaternion���g���ăx�N�g�����v���C���[�̑O�����ɉ�]������
    m_actor->GetRotation().Apply(toPos);

    // ��L�Ŏ擾������񂩂�A�U���͈͂𐶐�����|�W�V�������擾
    return toPos;
}

////////////////////////////////////////////////////////////
// �U���֘A
////////////////////////////////////////////////////////////

void Player::UpdateAttack()
{
    //m_attackData.UpdateContinuousAttack();
    m_attackData.Update();

    // �_�b�V���֘A��Update
    m_dashStatus.DashUpdate();

    // �m�b�N�o�b�N�֘A��Update
    m_squeezeStatus.SqueezeUpdate();

    // �_�E���֘A��Update
    m_downStatus.DownUpdate();

    // �K�[�h�֘A��Update
    m_defenceData.Update();

    // �U�������������Ƃ��̏���
    if (true == m_hitbox->UpdateCheckAttack()) {
        // �U������������
        HitAttack();
    }
}

void Player::HitAttack()
{    
    // �_���[�W����
    if (false == m_otherPlayer->ReceiveDamage(m_attackData.GetPower())) {
        // �_���[�W��^�����ĂȂ�
        return;
    }
}

void Player::AttackAnimationStart()
{
    // �U�����̃A�j���[�V�����̍Đ����J�n����֐�
    // ���̊֐��́A�ǂ������̏����ɕς�����
    // �iActor�N���X�̃C���X�^���X�ŏ���������悤�ɂ������j

    if (m_attackData.EnAttackType::enJub == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::jub);
        return;
    }
    if (m_attackData.EnAttackType::enUppercut == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::uppercut);
        return;
    }
    if (m_attackData.EnAttackType::enHook == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::hook);
        return;
    }
    if (m_attackData.EnAttackType::enBodyBlow == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::bodyBlow);
        return;
    }
    if (m_attackData.EnAttackType::enStraight == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::straight);
        return;
    }
}