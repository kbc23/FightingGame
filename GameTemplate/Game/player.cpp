#include "stdafx.h"
#include "player.h"

#include "constant.h"
#include "player_camera.h"
#include "attack_judgment.h"
#include "player_UI.h"
#include "game_data.h"

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

    m_attackData.UpdateContinuousAttack();

    // ダッシュ関連のUpdate
    m_dashStatus.DashUpdate();

    // ノックバック関連のUpdate
    m_squeezeStatus.SqueezeUpdate();

    // ダウン関連のUpdate
    m_downStatus.DownUpdate();

    //////////////////////////////
    // UIのUpdate
    //////////////////////////////

    m_playerUI->UpdateHpUI(m_hp, m_playerNum);
    m_playerUI->UpdateMpUI(m_mp, m_playerNum);
    m_playerUI->UpdateDashUI(m_dashStatus.GetRemainingNumberOfTimes(), m_playerNum);
    m_playerUI->UpdateKnockBackUI(m_squeezeStatus.GetFlagSqueeze(), m_playerNum);
    m_playerUI->UpdateDownUI(m_downStatus.GetFlagDown(), m_playerNum);
    m_playerUI->UpdatemDefenseUI(m_flagDefense, m_playerNum);
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
    if (true == m_attackData.GetFlagAttackNow()) {
        return;
    }

    // ノックバック時、処理をしない
    if (true == m_squeezeStatus.GetFlagSqueeze()) {
        return;
    }

    // ダウン時、処理をしない
    if (true == m_downStatus.GetFlagDown()) {
        return;
    }



    // Aボタン: 通常攻撃
    if (false == m_flagDefense && false == m_dashStatus.GetFlagDash() && true == m_gamePad->IsTrigger(enButtonA)) {
        m_attackData.SetAttackData(m_attackData.EnAttackType::enNormal);
    }
    // Bボタン: サブ攻撃
    if (false == m_flagDefense && false == m_dashStatus.GetFlagDash() && true == m_gamePad->IsTrigger(enButtonB)) {
        if (1 <= m_mp) {
            m_attackData.SetAttackData(m_attackData.EnAttackType::enSub);

            --m_mp;
        }
    }
    // ?: 必殺技
    if (true) {

    }
    // R1ボタン: ダッシュ
    if (false == m_flagDefense && true == m_gamePad->IsTrigger(enButtonRB1)) {
        m_dashStatus.StartDash();
    }
    // L1ボタン: ガード
    if (false == m_dashStatus.GetFlagDash() && true == m_gamePad->IsPress(enButtonLB1)) {
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

    // ガード中は処理をしない
    if (false == m_flagDefense) {
        if (false == m_dashStatus.GetFlagDash()) {
            moveAmount = Move();
        }
        // ダッシュ
        else {
            moveAmount = DashMove();
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
    float characterSpeed = 9.0f;

    //if (true == m_flagDefense) {
    //    characterSpeed = 3.0f;
    //}

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

const bool Player::AttackCreate()
{
    // 攻撃前のディレイがあるならここで処理を終了
    if (false == m_attackData.DelayAttack()) {
        return false;
    }

    // 攻撃範囲を作成
    m_attackJudgment->Create(
        m_attackData.CreateAttackPosition(m_actor->GetPosition(), m_actor->GetRotation()),
        m_actor->GetRotation(),
        m_attackData.GetRange()
    );

    return true;
}

void Player::AttackUpdate()
{
    // 攻撃中でない場合、処理をしない
    if (false == m_attackData.GetFlagAttackNow()) {
        return;
    }

    if (false == m_attackData.GetFlagCreateAttackRange()) {
        if (false == AttackCreate()) {
            return;
        }
    }

    // 攻撃が当たったか（攻撃がまだ当たっていないときのみ）
    if (false == m_attackData.GetFlagAlreadyAttacked() && true == m_attackJudgment->CheckHit()) {
        HitAttack();
    }

    if (true == m_attackData.UpdateFinish()) {
        FinishAttack();
    }
}

void Player::HitAttack()
{    
    // ダメージ処理
    if (false == m_otherPlayer->ReceiveDamage(m_attackData.GetPower(), m_attackData.GetImpactType())) {
        // ダメージを与えられてない
        return;
    }

    m_attackData.SetFlagAlreadyAttacked(true);

    m_mp += 0.5;

    if (m_MAX_MP <= m_mp) {
        m_mp = m_MAX_MP;
    }
}

void Player::FinishAttack()
{
    // 攻撃判定の削除
    m_attackJudgment->Release();

    // 攻撃時のステータスの初期化
    m_attackData.ResetAttackData();
}