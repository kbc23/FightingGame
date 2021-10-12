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
    GamePad& gamePad,
    const char* filePath,
    const Vector3& initPos,
    float initRotAngle,
    Actor* pOtherActor
)
{
    //m_gamePad = &gamePad;

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
    //if (m_gamePad->GetLStickXF() != 0.0f) {
    //    m_position.x -= m_gamePad->GetLStickXF() * 5.0f;
    //}
    //if (m_gamePad->GetLStickYF() != 0.0f) {
    //    m_position.z -= m_gamePad->GetLStickYF() * 5.0f;
    //}

    //if (m_gamePad->IsPress(enButtonA) == true) {
    //    m_rotY += 0.01f;
    //    m_rotation.SetRotationY(m_rotY);
    //}
    //if (m_gamePad->IsPress(enButtonB) == true) {
    //    m_rotY -= 0.01f;
    //    m_rotation.SetRotationY(m_rotY);
    //}

    SetModelStatus();
}

void Actor::AddStatus(Vector3& addMoveAmount, const float addRotAngle)
{
    m_position = m_charaCon.Execute(addMoveAmount, 1.0f);
    m_rotY += addRotAngle;
    m_rotation.SetRotationY(m_rotY);
}

void Actor::SetModelStatus()
{
    // 位置・回転・拡大情報を渡す
    m_modelCharacter->SetPosition(m_position);
    m_modelCharacter->SetRotation(m_rotation);
    m_modelCharacter->SetScale(m_scale);
}

void Actor::Move()
{
    // 現在の位置情報を保存
    Vector3 oldPos = m_position;

    // カメラの前方向を取得
    // ※カメラの前方向を参照する技を使うときに[cameraFront]が使えるかも
    Vector3 cameraFront = m_position - g_camera3D->GetPosition();
    cameraFront.y = 0.0f;
    cameraFront.Normalize();

    // カメラの右方向
    Vector3 cameraRight = Cross(g_vec3AxisY, cameraFront);

    float dot = cameraFront.Dot(Vector3::AxisZ); // 内積
    float angle = acosf(dot); // アークコサイン
    if (cameraFront.x < 0) {
        angle *= -1;
    }

    float characterSpeed = 6.0f; // キャラクターの移動速度

    Vector3 moveAmount = cameraFront * g_pad[0]->GetLStickYF() * characterSpeed + cameraRight * g_pad[0]->GetLStickXF() * characterSpeed;

    m_position = m_charaCon.Execute(moveAmount, 1.0f);
}