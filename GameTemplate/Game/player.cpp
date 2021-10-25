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
        const int POWER = 100; // 攻撃力
        const int ATTACK_TIME_LIMIT = 15; // 攻撃時間
        const Vector3 RANGE = { 100.0f,50.0f,50.0f }; // 攻撃範囲
        const float POSITION_UP_Y = 50.0f; // 攻撃範囲のY座標の調整
    }

    ////////////////////////////////////////////////////////////
    // サブ攻撃
    ////////////////////////////////////////////////////////////

    namespace nsSubAttack
    {
        const int POWER = 100; // 攻撃力
        const int ATTACK_TIME_LIMIT = 30; // 攻撃時間
        const int DELAY_TIME_LIMIT = 20; // 攻撃までのディレイ
        const Vector3 RANGE = { 50.0f,50.0f,500.0f }; // 攻撃範囲
        const float POSITION_UP_Y = 50.0f; // 攻撃範囲のY座標の調整
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

    // 攻撃関連のUpdate
    AttackUpdate();

    // ダッシュ関連のUpdate
    DashUpdate();

    // ノックバック関連のUpdate
    KnockBackUpdate();

    //////////////////////////////
    // UIのUpdate
    //////////////////////////////

    m_playerUI->UpdateHpUI(m_hp, m_playerNum);
    m_playerUI->UpdateDashUI(m_dashStatus.remainingNumberOfTimes, m_playerNum);
    m_playerUI->UpdateKnockBackUI(m_knockBackStatus.flagKnockBack, m_playerNum);
}

////////////////////////////////////////////////////////////
// 移動処理
////////////////////////////////////////////////////////////

void Player::Controller()
{
    // ゲーム終了時、処理をしない
    if (true == m_flagGameEndStopOperation) {
        return;
    }

    // 攻撃時、処理をしない
    if (false == m_flagOperation || true == m_attackData.flagAttackNow) {
        return;
    }

    // ノックバック時、処理をしない
    if (true == m_knockBackStatus.flagKnockBack) {
        return;
    }

    // Aボタン: 通常攻撃
    if (false == m_flagDefense && false == m_dashStatus.flagDash && true == m_gamePad->IsTrigger(enButtonA)) {
        // 攻撃判定のエリアを作成
        AttackCreate(EnAttackType::enNormal);
    }
    // B: サブ攻撃
    if (false == m_flagDefense && false == m_dashStatus.flagDash && true == m_gamePad->IsTrigger(enButtonB)) {
        // 攻撃判定のエリアを作成
        AttackCreate(EnAttackType::enSub);
    }
    // ?: 必殺技
    if (true) {

    }
    // R1ボタン: ダッシュ
    if (false == m_flagDefense && true == m_gamePad->IsTrigger(enButtonRB1)) {
        StartDash();
    }
    // L1ボタン: ガード
    if (false == m_dashStatus.flagDash && true == m_gamePad->IsPress(enButtonLB1)) {
        m_flagDefense = true;
    }
    else {
        m_flagDefense = false;
    }
    // Debug: Startボタン: ゲーム終了
    if (true == m_gamePad->IsTrigger(enButtonStart)) {
        //ゲームを終了
        exit(EXIT_SUCCESS);
    }

    // プレイヤーの移動
    Vector3 moveAmount = Vector3::Zero;

    if (false == m_dashStatus.flagDash) {
        moveAmount = Move();
    }
    // ダッシュ
    else {
        moveAmount = DashMove();
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
    float characterSpeed = 9.0f;

    if (true == m_flagDefense) {
        characterSpeed = 3.0f;
    }

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
        m_attackData.attackTimeLimit = nsAttackData::nsNormalAttack::ATTACK_TIME_LIMIT;
        m_attackData.Range = nsAttackData::nsNormalAttack::RANGE;
        m_attackData.positionUpY = nsAttackData::nsNormalAttack::POSITION_UP_Y;
        m_attackData.flagAttackNow = true;
        m_attackData.attackType = EnAttackType::enNormal;
    }
    else if (EnAttackType::enSub == attackType) {
        m_attackData.power = nsAttackData::nsSubAttack::POWER;
        m_attackData.attackTimeLimit = nsAttackData::nsSubAttack::ATTACK_TIME_LIMIT;
        m_attackData.delayTimeLimit = nsAttackData::nsSubAttack::DELAY_TIME_LIMIT;
        m_attackData.flagFinishDelay = false;
        m_attackData.Range = nsAttackData::nsSubAttack::RANGE;
        m_attackData.positionUpY = nsAttackData::nsSubAttack::POSITION_UP_Y;
        m_attackData.flagAttackNow = true;
        m_attackData.attackType = EnAttackType::enNormal;
    }

    // 攻撃前のディレイがあるならここで処理を終了
    if (false == m_attackData.flagFinishDelay) {
        return;
    }

    // 攻撃範囲を作成
    CreateAttackRange();
}

void Player::CreateAttackRange()
{
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
        m_actor->GetPosition().z - m_attackData.Range.z / 2 - 20.0f // カメラの前方向に攻撃範囲の前方向の半分の値を追加
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

    if (false == m_attackData.flagFinishDelay) {
        DelayAttack();
        return;
    }

    // 攻撃が当たったか（攻撃がまだ当たっていないときのみ）
    if (false == m_attackData.flagAlreadyAttacked && true == m_attackJudgment->CheckHit()) {
        HitAttack();
    }

    ++m_attackData.attackTime;

    if (m_attackData.attackTimeLimit <= m_attackData.attackTime) {
        ResetAttackData();
    }
}

void Player::DelayAttack()
{
    ++m_attackData.delayTime;

    if (m_attackData.delayTimeLimit <= m_attackData.delayTime) {
        // 攻撃範囲を作成
        CreateAttackRange();

        m_attackData.flagFinishDelay = true;
    }
}

void Player::HitAttack()
{
    // ここでは、相手プレイヤーが自分の攻撃判定に触れた際の処理を記載する
    // ダメージ処理
    if (false == m_otherPlayer->ReceiveDamage(m_attackData.power)) {
        // ダメージを与えられてない
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
    m_attackData.attackTime = 0;
    m_attackData.attackTimeLimit = 0;
    m_attackData.delayTime = 0;
    m_attackData.delayTimeLimit = 0;
    m_attackData.flagFinishDelay = true;
    m_attackData.Range = Vector3::Zero;
    m_attackData.positionUpY = 0.0f;
    m_attackData.flagAlreadyAttacked = false;
    m_attackData.flagAttackNow = false;
}

////////////////////////////////////////////////////////////
// ダッシュ関連
////////////////////////////////////////////////////////////

void Player::StartDash()
{
    // ダッシュ中か残り回数が０のときは処理をしない
    if (true == m_dashStatus.flagDash || 0 >= m_dashStatus.remainingNumberOfTimes) {
        return;
    }

    --m_dashStatus.remainingNumberOfTimes; // 残り回数を１減少

    m_dashStatus.flagDash = true;
}

void Player::DashUpdate()
{
    DashRecoveryTime();

    if (false == m_dashStatus.flagDash) {
        return;
    }

    ++m_dashStatus.countDash;

    if (m_dashStatus.MAX_COUNT_DASH <= m_dashStatus.countDash) {
        m_dashStatus.flagDash = false;
        m_dashStatus.countDash = 0;
    }
}

void Player::DashRecoveryTime()
{
    // 残り回数が最大なら処理をしない
    if (m_dashStatus.MAX_REMAINING_NUMBER_OF_TIMES <= m_dashStatus.remainingNumberOfTimes) {
        return;
    }

    ++m_dashStatus.countRecoveryTime;

    if (m_dashStatus.MAX_RECOVERY_TIME <= m_dashStatus.countRecoveryTime) {
        ++m_dashStatus.remainingNumberOfTimes; // 残り回数を１増加
        m_dashStatus.countRecoveryTime = 0;
    }
}

////////////////////////////////////////////////////////////
// ノックバック関連
////////////////////////////////////////////////////////////

void Player::StartKnockBack()
{
    m_knockBackStatus.flagKnockBack = true;
}

void Player::KnockBackUpdate()
{
    if (false == m_knockBackStatus.flagKnockBack) {
        return;
    }

    ++m_knockBackStatus.knockBackTime;

    if (m_knockBackStatus.knockBackTimeLimit <= m_knockBackStatus.knockBackTime) {
        m_knockBackStatus.knockBackTime = 0;
        m_knockBackStatus.flagKnockBack = false;
    }
}