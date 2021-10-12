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

    Vector3 moveAmount = { 0.0f,0.0f,0.0f }; // プレイヤーの移動量
    float rotY = 0.0f; // プレイヤーの回転量

    // プレイヤーの移動
    //if (m_gamePad->GetLStickXF() != 0.0f) {
    //    moveAmount = Move();
    //}
    //if (m_gamePad->GetLStickYF() != 0.0f) {
    //    moveAmount = Move();
    //}
    moveAmount = Move();
    // Aボタン: 通常攻撃
    if (m_gamePad->IsPress(enButtonA) == true) {
        // Aボタンを押したときの処理
        rotY += 0.01f; // 仮
    }
    // Bボタン: 特殊攻撃
    if (m_gamePad->IsPress(enButtonB) == true) {
        // Bボタンを押したときの処理
        rotY -= 0.01f; // 仮
    }
    // ?: 必殺技
    if (true) {

    }
    // ?: 回避
    if (true) {

    }
    // ?: ガード
    if (true) {

    }

    // プレイヤーのモデルに位置情報などのステータス情報を渡す
    m_actor->AddStatus(moveAmount, rotY);
}

const Vector3& Player::Move()
{
    // 現在の位置情報を保存
    Vector3 oldPos = m_actor->GetPosition();

    // カメラの前方向を取得
    // ※カメラの前方向を参照する技を使うときに[cameraFront]が使えるかも
    Vector3 cameraFront = m_actor->GetPosition() - g_camera3D->GetPosition();
    cameraFront.y = 0.0f;
    cameraFront.Normalize();

    // カメラの右方向
    Vector3 cameraRight = Cross(g_vec3AxisY, cameraFront);

    // キャラクターの移動速度
    float characterSpeed = 6.0f;

    Vector3 pospos = { 0.0f,0.0f,0.0f };

    if (m_gamePad->GetLStickXF() != 0.0f) {
        pospos.x -= m_gamePad->GetLStickXF() * characterSpeed;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        pospos.z -= m_gamePad->GetLStickYF() * characterSpeed;
    }

    //キャラクターの移動量を計算
    Vector3 moveAmount = cameraFront * pospos.z + cameraRight * pospos.x;

    return moveAmount;
}