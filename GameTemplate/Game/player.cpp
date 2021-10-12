#include "stdafx.h"
#include "player.h"

#include "constant.h"

Player::Player()
{

}

Player::~Player()
{

}

bool Player::Start()
{
    return true;
}

void Player::Init(
    GamePad& gamePad,
    const Vector3& initPos,
    float initRotAngle,
    Player* pOtherPlayer
)
{
    m_gamePad = &gamePad;
    m_otherPlayer = pOtherPlayer;

    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->Init(
        "Assets/modelData/unityChan.tkm",
        initPos,
        initRotAngle,
        &pOtherPlayer->GetActor()
    );
}

void Player::DebugInit(const char* filePath, const int playerNum, const Vector3& initPos, const float initRot)
{
    m_gamePad = g_pad[playerNum];
    
    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->DebugInit(filePath, playerNum, initPos, initRot);
}

void Player::Update()
{
    Controller();
}

void Player::Controller()
{
    if (false == m_flagOperation) {
        return;
    }

    Vector3 moveAmount = { 0.0f,0.0f,0.0f }; // �v���C���[�̈ړ���
    float rotY = 0.0f; // �v���C���[�̉�]��

    // �v���C���[�̈ړ�
    //if (m_gamePad->GetLStickXF() != 0.0f) {
    //    moveAmount = Move();
    //}
    //if (m_gamePad->GetLStickYF() != 0.0f) {
    //    moveAmount = Move();
    //}
    moveAmount = Move();
    // A�{�^��: �ʏ�U��
    if (m_gamePad->IsPress(enButtonA) == true) {
        // A�{�^�����������Ƃ��̏���
        rotY += 0.01f; // ��
    }
    // B�{�^��: ����U��
    if (m_gamePad->IsPress(enButtonB) == true) {
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
    Vector3 cameraFront = m_actor->GetPosition() - g_camera3D->GetPosition();
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