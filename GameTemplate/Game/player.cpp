#include "stdafx.h"
#include "player.h"

#include "actor.h"
#include "player_camera.h"

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
    Actor* pOtherActor
)
{
    m_gamePad = &gamePad;

    //// ���ׂẴv���C���[���������B
    //// ���f���֘A��NewGO
    //m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    ////m_actor[1] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    //const Vector3 pos[] = {
    //    {100.0f, 0.0f, 0.0f},		// 1P�̏������W
    //    {-100.0f, 0.0f, 0.0f},		// 2P�̏������W
    //};
    //float rotAngle[] = {
    //    -90.0f,
    //    90.0f
    //};

    //// �����̃v���C���[�ԍ����擾
    //int playerNo = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
    //int otherPlayerNo = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();
    //// ����
    //m_actor->Init(
    //    m_onlineTwoPlayerMatchEngine->GetGamePad(playerNo),
    //    "Assets/modelData/unityChan.tkm",
    //    pos[playerNo],
    //    rotAngle[playerNo],
    //    m_actor[otherPlayerNo]
    //);
    //// �ΐ푊��
    //m_actor[otherPlayerNo]->Init(
    //    m_onlineTwoPlayerMatchEngine->GetGamePad(otherPlayerNo),
    //    "Assets/modelData/unityChan.tkm",
    //    pos[otherPlayerNo],
    //    rotAngle[otherPlayerNo],
    //    m_actor[playerNo]
    //);
}

void Player::DebugInit(const char* filePath, const int playerNum, const Vector3& initPos)
{
    m_gamePad = g_pad[playerNum];

    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->DebugInit(filePath, playerNum, initPos);

    //m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    //m_modelCharacter->Init(filePath, false, true);

    //m_position = initPos;
    //m_modelCharacter->SetPosition(m_position);
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

    Vector3 position = { 0.0f,0.0f,0.0f };
    float rotY = 0.0f;

    // �v���C���[�̈ړ�
    if (m_gamePad->GetLStickXF() != 0.0f) {
        position.x -= m_gamePad->GetLStickXF() * 5.0f;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        position.z -= m_gamePad->GetLStickYF() * 5.0f;
    }
    // A�{�^��: �ʏ�U��
    if (m_gamePad->IsPress(enButtonA) == true) {
        // A�{�^�����������Ƃ��̏���
        rotY += 0.01f;
    }
    // B�{�^��: ����U��
    if (m_gamePad->IsPress(enButtonB) == true) {
        // B�{�^�����������Ƃ��̏���
        rotY -= 0.01f;
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

    //�v���C���[�̃��f���Ɉʒu���Ȃǂ̃X�e�[�^�X����n��
    m_actor->AddStatus(position, rotY);
}