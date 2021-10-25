#include "stdafx.h"
#include "player.h"

#include "constant.h"
#include "player_camera.h"
#include "attack_judgment.h"
#include "player_UI.h"
#include "game_data.h"







Player::Player()
{

}

Player::~Player()
{
    DeleteGO(m_actor);
    DeleteGO(m_attackJudgment);
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
    m_gamePad = &gamePad;
    m_playerNum = playerNum;
    m_otherPlayer = pOtherPlayer;

    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->Init(
        "Assets/modelData/unityChan.tkm",
        initPos,
        initRotAngle
    );

    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);

    m_attackJudgment = NewGO<AttackJudgment>(igo::EnPriority::normal);
    m_attackJudgment->Init(playerNum);

    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

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
    m_gamePad = g_pad[playerNum];
    m_playerNum = playerNum;
    m_otherPlayer = pOtherPlayer;
    
    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->DebugInit(filePath, initPos, initRot);

    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);

    m_attackJudgment = NewGO<AttackJudgment>(igo::EnPriority::normal);
    m_attackJudgment->Init(playerNum);

    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);
}

void Player::Update()
{
    // ����
    Controller();

    // �U���֘A��Update
    AttackUpdate();

    // �_�b�V���֘A��Update
    DashUpdate();

    // �m�b�N�o�b�N�֘A��Update
    KnockBackUpdate();

    //////////////////////////////
    // UI��Update
    //////////////////////////////

    m_playerUI->UpdateHpUI(m_hp, m_playerNum);
    m_playerUI->UpdateDashUI(m_dashStatus.remainingNumberOfTimes, m_playerNum);
    m_playerUI->UpdateKnockBackUI(m_knockBackStatus.flagKnockBack, m_playerNum);
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
    if (true == m_knockBackStatus.flagKnockBack) {
        return;
    }

    // A�{�^��: �ʏ�U��
    if (false == m_flagDefense && false == m_dashStatus.flagDash && true == m_gamePad->IsTrigger(enButtonA)) {
        // �U������̃G���A���쐬
        //AttackCreate(m_attackData.EnAttackType::enNormal);
        m_attackData.SetAttackData(m_attackData.EnAttackType::enNormal);
    }
    // B: �T�u�U��
    if (false == m_flagDefense && false == m_dashStatus.flagDash && true == m_gamePad->IsTrigger(enButtonB)) {
        // �U������̃G���A���쐬
        //AttackCreate(m_attackData.EnAttackType::enSub);
        m_attackData.SetAttackData(m_attackData.EnAttackType::enSub);
    }
    // ?: �K�E�Z
    if (true) {

    }
    // R1�{�^��: �_�b�V��
    if (false == m_flagDefense && true == m_gamePad->IsTrigger(enButtonRB1)) {
        StartDash();
    }
    // L1�{�^��: �K�[�h
    if (false == m_dashStatus.flagDash && true == m_gamePad->IsPress(enButtonLB1)) {
        m_flagDefense = true;
    }
    else {
        m_flagDefense = false;
    }
    // Debug: Start�{�^��: �Q�[���I��
    if (true == m_gamePad->IsTrigger(enButtonStart)) {
        //�Q�[�����I��
        exit(EXIT_SUCCESS);
    }

    // �v���C���[�̈ړ�
    Vector3 moveAmount = Vector3::Zero;

    if (false == m_dashStatus.flagDash) {
        moveAmount = Move();
    }
    // �_�b�V��
    else {
        moveAmount = DashMove();
    }

    // �v���C���[�̃��f���Ɉʒu���Ȃǂ̃X�e�[�^�X����n��
    m_actor->AddStatus(moveAmount);
}

const Vector3& Player::Move()
{
    // ���݂̈ʒu����ۑ�
    Vector3 oldPos = m_actor->GetPosition();

    // �J�����̑O�������擾
    // ���J�����̑O�������Q�Ƃ���Z���g���Ƃ���[cameraFront]���g���邩��
    Vector3 cameraFront = m_actor->GetPosition() - m_findPlayerCamera->GetPosition(m_playerNum);
    cameraFront.y = 0.0f;
    cameraFront.Normalize();

    // �J�����̉E����
    Vector3 cameraRight = Cross(g_vec3AxisY, cameraFront);

    // �L�����N�^�[�̈ړ����x
    float characterSpeed = 9.0f;

    if (true == m_flagDefense) {
        characterSpeed = 3.0f;
    }

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

const Vector3& Player::DashMove()
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

const bool Player::AttackCreate()
{
    // �U���O�̃f�B���C������Ȃ炱���ŏ������I��
    if (false == m_attackData.DelayAttack()) {
        return false;
    }

    // �U���͈͂��쐬
    m_attackJudgment->Create(
        m_attackData.CreateAttackPosition(m_actor->GetPosition(), m_actor->GetRotation()),
        m_actor->GetRotation(),
        m_attackData.GetRange()
    );

    return true;
}

void Player::AttackUpdate()
{
    // �U�����łȂ��ꍇ�A���������Ȃ�
    if (false == m_attackData.GetFlagAttackNow()) {
        return;
    }

    if (false == m_attackData.GetFlagCreateAttackRange()) {
        if (false == AttackCreate()) {
            return;
        }
    }

    // �U���������������i�U�����܂��������Ă��Ȃ��Ƃ��̂݁j
    if (false == m_attackData.GetFlagAlreadyAttacked() && true == m_attackJudgment->CheckHit()) {
        HitAttack();
    }

    if (true == m_attackData.UpdateFinish()) {
        FinishAttack();
    }
}

void Player::HitAttack()
{
    // �����ł́A����v���C���[�������̍U������ɐG�ꂽ�ۂ̏������L�ڂ���
    // �_���[�W����
    if (false == m_otherPlayer->ReceiveDamage(m_attackData.GetPower())) {
        // �_���[�W��^�����ĂȂ�
        return;
    }

    m_attackData.SetFlagAlreadyAttacked(true);
}

void Player::FinishAttack()
{
    // �U������̍폜
    m_attackJudgment->Release();

    // �U�����̃X�e�[�^�X�̏�����
    m_attackData.ResetAttackData();
}

////////////////////////////////////////////////////////////
// �_�b�V���֘A
////////////////////////////////////////////////////////////

void Player::StartDash()
{
    // �_�b�V�������c��񐔂��O�̂Ƃ��͏��������Ȃ�
    if (true == m_dashStatus.flagDash || 0 >= m_dashStatus.remainingNumberOfTimes) {
        return;
    }

    --m_dashStatus.remainingNumberOfTimes; // �c��񐔂��P����

    m_dashStatus.flagDash = true;
}

void Player::DashUpdate()
{
    DashRecoveryTime();

    if (false == m_dashStatus.flagDash) {
        return;
    }

    ++m_dashStatus.countDash;

    if (m_dashStatus.MAX_COUNT_DASH <= m_dashStatus.countDash) {
        m_dashStatus.flagDash = false;
        m_dashStatus.countDash = 0;
    }
}

void Player::DashRecoveryTime()
{
    // �c��񐔂��ő�Ȃ珈�������Ȃ�
    if (m_dashStatus.MAX_REMAINING_NUMBER_OF_TIMES <= m_dashStatus.remainingNumberOfTimes) {
        return;
    }

    ++m_dashStatus.countRecoveryTime;

    if (m_dashStatus.MAX_RECOVERY_TIME <= m_dashStatus.countRecoveryTime) {
        ++m_dashStatus.remainingNumberOfTimes; // �c��񐔂��P����
        m_dashStatus.countRecoveryTime = 0;
    }
}

////////////////////////////////////////////////////////////
// �m�b�N�o�b�N�֘A
////////////////////////////////////////////////////////////

void Player::StartKnockBack()
{
    m_knockBackStatus.flagKnockBack = true;
}

void Player::KnockBackUpdate()
{
    if (false == m_knockBackStatus.flagKnockBack) {
        return;
    }

    ++m_knockBackStatus.knockBackTime;

    if (m_knockBackStatus.knockBackTimeLimit <= m_knockBackStatus.knockBackTime) {
        m_knockBackStatus.knockBackTime = 0;
        m_knockBackStatus.flagKnockBack = false;
    }
}