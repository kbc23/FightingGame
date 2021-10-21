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
    float initRotAngle,
    Actor* pOtherActor
)
{
    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true);
    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    m_otherActor = pOtherActor;

    //キャラコンの初期化
    m_charaCon.Init(PLAYER_COLLIDER_RADIUS, PLAYER_COLLIDER_HEIGHT, m_position);
}

void Actor::DebugInit(const char* filePath, const int playerNum, const Vector3& initPos, const float initRot)
{
    //m_gamePad = g_pad[playerNum];

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true);

    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    m_rotY = initRot;
    m_rotation.SetRotationY(m_rotY);
    m_modelCharacter->SetRotation(m_rotation);

    //キャラコンの初期化
    m_charaCon.Init(PLAYER_COLLIDER_RADIUS, PLAYER_COLLIDER_HEIGHT, m_position);
}

void Actor::Update()
{
    SetModelStatus();
}

void Actor::AddStatus(Vector3& addMoveAmount)
{
    // キャラクターの位置を決定
    m_position = m_charaCon.Execute(addMoveAmount, 1.0f);
    // キャラクターの向きを決定
    Turn(addMoveAmount);
}

void Actor::Turn(Vector3& addMoveAmount)
{
    if (fabsf(addMoveAmount.x) < 0.001f
        && fabsf(addMoveAmount.z) < 0.001f) {
        // m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
        // このフレームではキャラは移動していないので旋回する必要はない
        return;
    }
    // atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数
    // m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる
    // atan2を使用して、角度を求めている
    // これが回転角度になる
    float angle = atan2(addMoveAmount.x, addMoveAmount.z);
    // atanが返してくる角度はラジアン単位なので
    // SetRotationDegではなくSetRotationを使用する
    m_rotation.SetRotation(Vector3::AxisY, angle);

}

void Actor::SetModelStatus()
{
    // 位置・回転・拡大情報を渡す
    m_modelCharacter->SetPosition(m_position);
    m_modelCharacter->SetRotation(m_rotation);
    m_modelCharacter->SetScale(m_scale);
}