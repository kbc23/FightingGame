#include "stdafx.h"
#include "player.h"

#include "constant.h"
#include "player_camera.h"
#include "attack_judgment.h"
#include "player_UI.h"
#include "game_data.h"



namespace nsAttackData
{
    ////////////////////////////////////////////////////////////
    // 通常攻撃
    ////////////////////////////////////////////////////////////

    namespace nsNormalAttack
    {
        const int POWER = 100;
        const int TIME_LIMIT = 5;
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
    DeleteGO(m_playerUI);
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
        initRotAngle
    );

    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);

    m_attackJudgment = NewGO<AttackJudgment>(igo::EnPriority::normal);
    m_attackJudgment->Init(playerNum);

    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);
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
    m_actor->DebugInit(filePath, initPos, initRot);

    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);

    m_attackJudgment = NewGO<AttackJudgment>(igo::EnPriority::normal);
    m_attackJudgment->Init(playerNum);

    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);
}

void Player::Update()
{
    // 操作
    Controller();

    // Debug start
    //if (m_findGameData->GetOtherPlayerNum() == m_playerNum) {
    //    AttackCreate(EnAttackType::enNormal);
    //}
    // Debug end

    // 攻撃関連のUpdate
    AttackUpdate();

    //////////////////////////////
    // UIのUpdate
    //////////////////////////////

    m_playerUI->UpdateHpUI(m_hp, m_playerNum);
}

////////////////////////////////////////////////////////////
// 移動処理
////////////////////////////////////////////////////////////

void Player::Controller()
{
    if (true == m_flagGameEndStopOperation) {
        return;
    }

    if (false == m_flagOperation || true == m_attackData.flagAttackNow) {
        return;
    }

    // Aボタン: 通常攻撃
    if (true == m_gamePad->IsTrigger(enButtonA)) {
        // 攻撃判定のエリアを作成
        AttackCreate(EnAttackType::enNormal);
    }
    // ?: 特殊攻撃
    if (true) {
    }
    // ?: 必殺技
    if (true) {

    }
    // Xボタン（仮）: ダッシュ
    if (true == m_gamePad->IsTrigger(enButtonX)) {
        m_flagDash = true;
    }
    // Bボタン（仮）: ガード
    if (true == m_gamePad->IsPress(enButtonB)) {
        m_flagDefense = true;
    }
    else {
        m_flagDefense = false;
    }

    // プレイヤーの移動
    Vector3 moveAmount = Vector3::Zero;

    if (false == m_flagDash) {
        moveAmount = Move();
    }
    // ダッシュ
    else {
        moveAmount = DashMove();

        ++m_countDash;

        if (5 <= m_countDash) {
            m_flagDash = false;
            m_countDash = 0;
        }
    }

    // プレイヤーのモデルに位置情報などのステータス情報を渡す
    m_actor->AddStatus(moveAmount);
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

    Vector3 pospos = Vector3::Zero;

    if (m_gamePad->GetLStickXF() != 0.0f) {
        pospos.x += m_gamePad->GetLStickXF() * characterSpeed;
    }
    if (m_gamePad->GetLStickYF() != 0.0f) {
        pospos.z += m_gamePad->GetLStickYF() * characterSpeed;
    }

    //キャラクターの移動量を計算
    Vector3 moveAmount = cameraFront * pospos.z + cameraRight * pospos.x;

    return moveAmount;
}

const Vector3& Player::DashMove()
{
    // ダッシュのために仮で作成するポジション
    Vector3 attackRangePosition =
    {
        m_actor->GetPosition().x,
        m_actor->GetPosition().y,
        m_actor->GetPosition().z - 1.0f // カメラの前方向に少しだけ奥に置く
    };
    // キャラクターのポジション
    Vector3 playerPosition = m_actor->GetPosition();

    // 仮のポジションからキャラクターのポジションのベクトルを取得
    Vector3 toPos = playerPosition - attackRangePosition;

    // キャラクターの移動速度
    float characterSpeed = 80.0f;

    // キャラクターの移動量の計算
    toPos.z = toPos.z * characterSpeed;
    // キャラクターのQuaternionを使ってベクトルをプレイヤーの前方向に回転させる
    m_actor->GetRotation().Apply(toPos);

    // 上記で取得した情報から、攻撃範囲を生成するポジションを取得
    return toPos;
}

////////////////////////////////////////////////////////////
// 攻撃関連
////////////////////////////////////////////////////////////

void Player::AttackCreate(const int attackType)
{
    // 通常攻撃
    if (EnAttackType::enNormal == attackType) {
        m_attackData.power = nsAttackData::nsNormalAttack::POWER;
        m_attackData.timeLimit = nsAttackData::nsNormalAttack::TIME_LIMIT;
        m_attackData.Range = nsAttackData::nsNormalAttack::RANGE;
        m_attackData.positionUpY = nsAttackData::nsNormalAttack::POSITION_UP_Y;
        m_attackData.flagAttackNow = true;
        m_attackData.attackType = EnAttackType::enNormal;
    }

    // キャラクターの前方を攻撃範囲にする
    // キャラクターの前方向を取得して生成位置を変更する
    Vector3 createPosition = CreateAttackPosition();

    // 指定した値だけY座標を上昇する
    createPosition.y = m_attackData.positionUpY;

    // 攻撃範囲を生成
    m_attackJudgment->Create(createPosition, m_actor->GetRotation(), m_attackData.Range);
}

const Vector3& Player::CreateAttackPosition()
{
    // 生成する攻撃範囲のポジション
    // 前方向に攻撃範囲の前方向の半分の値を追加する
    Vector3 attackRangePosition =
    {
        m_actor->GetPosition().x,
        m_actor->GetPosition().y,
        m_actor->GetPosition().z - m_attackData.Range.z / 2 // カメラの前方向に攻撃範囲の前方向の半分の値を追加
    };
    // キャラクターのポジション
    Vector3 playerPosition = m_actor->GetPosition();

    // 攻撃範囲のポジションからキャラクターのポジションのベクトルを取得
    Vector3 toPos = attackRangePosition - playerPosition;
    // キャラクターのQuaternionを使ってベクトルをプレイヤーの前方向に回転させる
    m_actor->GetRotation().Apply(toPos);

    // 上記で取得した情報から、攻撃範囲を生成するポジションを取得
    return playerPosition - toPos;
}

void Player::AttackUpdate()
{
    if (false == m_attackData.flagAttackNow) {
        return;
    }

    // 攻撃が当たったか（攻撃がまだ当たっていないときのみ）
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
    // ここでは、相手プレイヤーが自分の攻撃判定に触れた際の処理を記載する
    // ダメージ処理
    if (false == m_otherPlayer->ReceiveDamage(m_attackData.power)) {
        return;
    }

    m_attackData.flagAlreadyAttacked = true;
}

void Player::ResetAttackData()
{
    // 攻撃判定の削除
    m_attackJudgment->Release();

    // 攻撃時のステータスの初期化
    m_attackData.power = 0;
    m_attackData.time = 0;
    m_attackData.timeLimit = 0;
    m_attackData.Range = Vector3::Zero;
    m_attackData.positionUpY = 0.0f;
    m_attackData.flagAlreadyAttacked = false;
    m_attackData.flagAttackNow = false;
}