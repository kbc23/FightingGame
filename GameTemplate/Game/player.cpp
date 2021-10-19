#include "stdafx.h"
#include "player.h"

#include "constant.h"
#include "player_camera.h"
#include "attack_judgment.h"

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
}

void Player::Update()
{
    Controller();

    // Debug start
    float rotY = 0.0f;
    rotY = m_attackJudgment->DebugRotation();
    if (0.0f != rotY) {
        DebugHitAttack(rotY);
    }

    if (true == m_flagAttack) {
        ++m_attackTime;
        if (120 <= m_attackTime) {
            m_attackJudgment->Release(); // �U������̍폜
            m_flagAttack = false;
            m_attackTime = 0;
        }
    }
    // Debug end
}

void Player::Controller()
{
    if (false == m_flagOperation) {
        return;
    }

    Vector3 moveAmount = { 0.0f,0.0f,0.0f }; // �v���C���[�̈ړ���
    float rotY = 0.0f; // �v���C���[�̉�]��

    // �v���C���[�̈ړ�
    moveAmount = Move();

    // A�{�^��: �ʏ�U��
    if (true == m_gamePad->IsPress(enButtonA) && false == m_flagAttack) {
        // �U������̃G���A���쐬
        m_attackJudgment->Create(m_actor->GetPosition(), m_actor->GetRotation(), { 100.0f, 100.0f, 100.0f });
        m_flagAttack = true;
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

void Turn();

void Player::HitAttack()
{
    // �����ł́A����v���C���[�������̍U������ɐG�ꂽ�ۂ̏������L�ڂ���
}

void Player::DebugHitAttack(const float rotY)
{
    Vector3 moveAmount = { 0.0f,0.0f,0.0f }; // �v���C���[�̈ړ���

    m_otherPlayer->GetActor().AddStatus(moveAmount, rotY);
}