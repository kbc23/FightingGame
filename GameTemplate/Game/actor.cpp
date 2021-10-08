#include "stdafx.h"
#include "actor.h"



namespace
{
    const enum PlayerNum
    {
        enPlayer_1, //プレイヤー１
        enPlayer_2  //プレイヤー２
    };
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
    GamePad& gamePad,
    const char* filePath,
    const Vector3& initPos,
    float initRotAngle,
    Actor* pOtherActor
)
{
    m_gamePad = &gamePad;

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true);
    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    m_otherActor = pOtherActor;
}

void Actor::DebugInit(const char* filePath, const int playerNum, const Vector3& initPos)
{
    m_gamePad = g_pad[playerNum];

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true);

    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
}

void Actor::Update()
{
    if (m_gamePad->GetLStickXF() != 0.0f) {
        m_position.x -= m_gamePad->GetLStickXF() * 5.0f;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        m_position.z -= m_gamePad->GetLStickYF() * 5.0f;
    }

    if (m_gamePad->IsPress(enButtonA) == true) {
        m_rotY += 0.01f;
        m_rotation.SetRotationY(m_rotY);
    }
    if (m_gamePad->IsPress(enButtonB) == true) {
        m_rotY -= 0.01f;
        m_rotation.SetRotationY(m_rotY);
    }

    SetModelStatus();
}

void Actor::Controller()
{
    if (false == m_flagOperation) {
        return;
    }

    // プレイヤーの移動
    if (m_gamePad->GetLStickXF() != 0.0f) {
        m_position.x -= m_gamePad->GetLStickXF() * 5.0f;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        m_position.z -= m_gamePad->GetLStickYF() * 5.0f;
    }



    // Aボタン: 通常攻撃
    if (m_gamePad->IsPress(enButtonA) == true) {
        // Aボタンを押したときの処理

    }
    // Bボタン: 特殊攻撃
    if (m_gamePad->IsPress(enButtonB) == true) {
        // Bボタンを押したときの処理

    }
    //?: 必殺技
    if (true) {

    }
    // ?: 回避
    if (true) {

    }
    // ?: ガード
    if (true) {

    }
}

void Actor::SetModelStatus()
{
    // 位置・回転・拡大情報を渡す
    m_modelCharacter->SetPosition(m_position);
    m_modelCharacter->SetRotation(m_rotation);
    m_modelCharacter->SetScale(m_scale);
}