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
            m_attackJudgment->Release(); // 攻撃判定の削除
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

    Vector3 moveAmount = { 0.0f,0.0f,0.0f }; // プレイヤーの移動量
    float rotY = 0.0f; // プレイヤーの回転量

    // プレイヤーの移動
    moveAmount = Move();

    // Aボタン: 通常攻撃
    if (true == m_gamePad->IsPress(enButtonA) && false == m_flagAttack) {
        // 攻撃判定のエリアを作成
        m_attackJudgment->Create(m_actor->GetPosition(), m_actor->GetRotation(), { 100.0f, 100.0f, 100.0f });
        m_flagAttack = true;
    }
    // Bボタン: 特殊攻撃
    if (true == m_gamePad->IsPress(enButtonB)) {
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
    Vector3 cameraFront = m_actor->GetPosition() - m_findPlayerCamera->GetPosition(m_playerNum);
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

void Turn();

void Player::HitAttack()
{
    // ここでは、相手プレイヤーが自分の攻撃判定に触れた際の処理を記載する
}

void Player::DebugHitAttack(const float rotY)
{
    Vector3 moveAmount = { 0.0f,0.0f,0.0f }; // プレイヤーの移動量

    m_otherPlayer->GetActor().AddStatus(moveAmount, rotY);
}