#include "stdafx.h"
#include "player.h"

#include "constant.h"
#include "player_camera.h"
#include "attack_judgment.h"
#include "player_UI.h"



namespace nsAttackData
{
    ////////////////////////////////////////////////////////////
    // �ʏ�U��
    ////////////////////////////////////////////////////////////

    namespace nsNormalAttack
    {
        const int POWER = 100;
        const int TIME_LIMIT = 30;
        const Vector3 RANGE = { 100.0f,100.0f,100.0f };
        const float POSITION_UP_Y = 50.0f;
    }
}



Player::Player()
{

}

Player::~Player()
{
    DeleteGO(m_actor);
    DeleteGO(m_attackJudgment);
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
        initRotAngle,
        &pOtherPlayer->GetActor()
    );

    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);

    m_attackJudgment = NewGO<AttackJudgment>(igo::EnPriority::normal);
    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);
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
    m_actor->DebugInit(filePath, playerNum, initPos, initRot);

    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);

    m_attackJudgment = NewGO<AttackJudgment>(igo::EnPriority::normal);
    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);
}

void Player::Update()
{
    Controller();

    AttackUpdate();

    //////////////////////////////
    // UI��Update
    //////////////////////////////

    m_playerUI->UpdateHpUI(m_hp);
}

void Player::Controller()
{
    if (false == m_flagOperation || true == m_attackData.flagAttackNow) {
        return;
    }

    Vector3 moveAmount = { 0.0f,0.0f,0.0f }; // �v���C���[�̈ړ���
    float rotY = 0.0f; // �v���C���[�̉�]��

    // �v���C���[�̈ړ�
    moveAmount = Move();

    // A�{�^��: �ʏ�U��
    if (true == m_gamePad->IsPress(enButtonA)) {
        // �U������̃G���A���쐬
        AttackCreate(EnAttackType::normal);
    }
    // B�{�^��: ����U��
    if (true == m_gamePad->IsPress(enButtonB)) {
        // B�{�^�����������Ƃ��̏���
        rotY -= 0.01f; // ��
    }
    // ?: �K�E�Z
    if (true) {

    }
    // ?: ���
    if (true) {

    }
    // ?: �K�[�h
    if (true) {

    }

    // �v���C���[�̃��f���Ɉʒu���Ȃǂ̃X�e�[�^�X����n��
    m_actor->AddStatus(moveAmount, rotY);
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
    float characterSpeed = 6.0f;

    Vector3 pospos = { 0.0f,0.0f,0.0f };

    if (m_gamePad->GetLStickXF() != 0.0f) {
        pospos.x -= m_gamePad->GetLStickXF() * characterSpeed;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        pospos.z -= m_gamePad->GetLStickYF() * characterSpeed;
    }

    //�L�����N�^�[�̈ړ��ʂ��v�Z
    Vector3 moveAmount = cameraFront * pospos.z + cameraRight * pospos.x;

    return moveAmount;
}

////////////////////////////////////////////////////////////
// �U���֘A
////////////////////////////////////////////////////////////

void Player::AttackCreate(const int attackType)
{
    // �ʏ�U��
    if (EnAttackType::normal == attackType) {
        m_attackData.power = nsAttackData::nsNormalAttack::POWER;
        m_attackData.timeLimit = nsAttackData::nsNormalAttack::TIME_LIMIT;
        m_attackData.Range = nsAttackData::nsNormalAttack::RANGE;
        m_attackData.positionUpY = nsAttackData::nsNormalAttack::POSITION_UP_Y;
        m_attackData.flagAttackNow = true;
        m_attackData.attackType = EnAttackType::normal;
    }

    // �L�����N�^�[�̑O�����U���͈͂ɂ���
    // �L�����N�^�[�̑O�������擾���Đ����ʒu��ύX����
    Vector3 createPosition = CreateAttackPosition();

    // �w�肵���l����Y���W���㏸����
    createPosition.y = m_attackData.positionUpY;

    // �U���͈͂𐶐�
    m_attackJudgment->Create(createPosition, m_actor->GetRotation(), m_attackData.Range);
}

const Vector3& Player::CreateAttackPosition()
{
    // ��������U���͈͂̃|�W�V����
    // �O�����ɍU���͈͂̑O�����̔����̒l��ǉ�����
    Vector3 attackRangePosition =
    {
        m_actor->GetPosition().x,
        m_actor->GetPosition().y,
        m_actor->GetPosition().z - m_attackData.Range.z / 2 // �J�����̑O�����ɍU���͈͂̑O�����̔����̒l��ǉ�
    };
    // �L�����N�^�[�̃|�W�V����
    Vector3 playerPosition = m_actor->GetPosition();

    // �U���͈͂̃|�W�V��������L�����N�^�[�̃|�W�V�����̃x�N�g�����擾
    Vector3 toPos = attackRangePosition - playerPosition;
    // �L�����N�^�[��Quaternion���g���ăx�N�g�����v���C���[�̑O�����ɉ�]������
    m_actor->GetRotation().Apply(toPos);

    // ��L�Ŏ擾������񂩂�A�U���͈͂𐶐�����|�W�V�������擾
    return playerPosition - toPos;
}

void Player::AttackUpdate()
{
    if (false == m_attackData.flagAttackNow) {
        return;
    }

    // �U���������������i�U�����܂��������Ă��Ȃ��Ƃ��̂݁j
    if (false == m_attackData.flagAlreadyAttacked && true == m_attackJudgment->CheckHit()) {
        HitAttack();
    }

    ++m_attackData.time;

    if (m_attackData.timeLimit <= m_attackData.time) {
        ResetAttackData();
    }
}

void Player::HitAttack()
{
    // �����ł́A����v���C���[�������̍U������ɐG�ꂽ�ۂ̏������L�ڂ���
    m_otherPlayer->ReceiveDamage(m_attackData.power);

    m_attackData.flagAlreadyAttacked = true;
}

void Player::DebugHitAttack(const float rotY)
{
    Vector3 moveAmount = { 0.0f,0.0f,0.0f }; // �v���C���[�̈ړ���

    m_otherPlayer->GetActor().AddStatus(moveAmount, rotY);
}

void Player::ResetAttackData()
{
    m_attackJudgment->Release(); // �U������̍폜

    m_attackData.power = 0;
    m_attackData.time = 0;
    m_attackData.timeLimit = 0;
    m_attackData.Range = Vector3::Zero;
    m_attackData.positionUpY = 0.0f;
    m_attackData.flagAlreadyAttacked = false;
    m_attackData.flagAttackNow = false;
}