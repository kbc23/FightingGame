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

    //// すべてのプレイヤーが揃った。
    //// モデル関連のNewGO
    //m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    ////m_actor[1] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    //const Vector3 pos[] = {
    //    {100.0f, 0.0f, 0.0f},		// 1Pの初期座標
    //    {-100.0f, 0.0f, 0.0f},		// 2Pの初期座標
    //};
    //float rotAngle[] = {
    //    -90.0f,
    //    90.0f
    //};

    //// 自分のプレイヤー番号を取得
    //int playerNo = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
    //int otherPlayerNo = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();
    //// 自分
    //m_actor->Init(
    //    m_onlineTwoPlayerMatchEngine->GetGamePad(playerNo),
    //    "Assets/modelData/unityChan.tkm",
    //    pos[playerNo],
    //    rotAngle[playerNo],
    //    m_actor[otherPlayerNo]
    //);
    //// 対戦相手
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

    // プレイヤーの移動
    if (m_gamePad->GetLStickXF() != 0.0f) {
        position.x -= m_gamePad->GetLStickXF() * 5.0f;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        position.z -= m_gamePad->GetLStickYF() * 5.0f;
    }
    // Aボタン: 通常攻撃
    if (m_gamePad->IsPress(enButtonA) == true) {
        // Aボタンを押したときの処理
        rotY += 0.01f;
    }
    // Bボタン: 特殊攻撃
    if (m_gamePad->IsPress(enButtonB) == true) {
        // Bボタンを押したときの処理
        rotY -= 0.01f;
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

    //プレイヤーのモデルに位置情報などのステータス情報を渡す
    m_actor->AddStatus(position, rotY);
}