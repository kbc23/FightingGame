#include "stdafx.h"
#include "player.h"

#include "constant.h"
#include "player_camera.h"
#include "player_UI.h"
#include "game_data.h"
#include "attack_move_bone.h"

Player::Player()
{

}

Player::~Player()
{
    DeleteGO(m_actor);
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
    // 引数で持ってきた物をこのクラスで保持
    m_gamePad = &gamePad;
    m_playerNum = playerNum;
    m_otherPlayer = pOtherPlayer;

    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    // Actorクラスの初期化
    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->Init(
        "Assets/modelData/unityChan.tkm",
        initPos,
        initRotAngle
    );

    // 当たり判定の初期化
    m_hitbox = NewGO<Hitbox>(igo::EnPriority::normal);
    //m_hitbox->Init(*m_otherPlayer, *m_actor, m_attackData, m_defenceData);

    // プレイヤーに関係するUIクラスの初期化
    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    //////////////////////////////
    // FindGO
    //////////////////////////////

    // プレイヤーのカメラ
    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);
    // ゲームデータ
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
    // 引数で持ってきた物をこのクラスで保持
    m_gamePad = g_pad[playerNum];
    m_playerNum = playerNum;
    m_otherPlayer = pOtherPlayer;

    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////
    
    m_actor = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
    m_actor->DebugInit(filePath, initPos, initRot);

    m_hitbox = NewGO<Hitbox>(igo::EnPriority::normal);
    m_hitbox->Init(*m_otherPlayer, *m_actor, m_attackData, m_defenceData);

    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_findPlayerCamera = FindGO<PlayerCamera>(igo::className::PLAYER_CAMERA);

    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);
}

void Player::Update()
{
    // 操作
    Controller();

    // 攻撃関連の毎フレームの処理
    UpdateAttack();

    //////////////////////////////
    // UIのUpdate
    //////////////////////////////

    m_playerUI->UpdateHpUI(m_hp, m_playerNum);
    m_playerUI->UpdateDashUI(m_dashStatus.GetRemainingNumberOfTimes(), m_playerNum);
    m_playerUI->UpdateKnockBackUI(m_squeezeStatus.GetFlagSqueeze(), m_playerNum);
    m_playerUI->UpdateDownUI(m_downStatus.GetFlagDown(), m_playerNum);
    m_playerUI->UpdateDefenseUI(m_defenceData.GetFlagDefense(), m_playerNum);
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

    //////////////////////////////
    // ボタン入力
    //////////////////////////////

    // Aボタン
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonA)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enJub)) {
            AttackAnimationStart();
        }
    }
    // Bボタン
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonB)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enHook)) {
            AttackAnimationStart();
        }
    }
    // Xボタン
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonX)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enUppercut)) {
            AttackAnimationStart();
        }
    }
    // Yボタン
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonY)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enStraight)) {
            AttackAnimationStart();
        }
    }
    // R2ボタン
    if (false == m_defenceData.GetFlagDefense() &&
        false == m_dashStatus.GetFlagDash() &&
        true == m_gamePad->IsTrigger(enButtonRB2)) {
        if (true == m_attackData.SetAttackData(m_attackData.EnAttackType::enBodyBlow)) {
            AttackAnimationStart();
        }
    }

    // R1ボタン: ダッシュ
    if (false == m_defenceData.GetFlagDefense() && true == m_gamePad->IsTrigger(enButtonRB1)) {
        m_dashStatus.StartDash();
    }

    // L1ボタン: ガード
    if (false == m_dashStatus.GetFlagDash() && true == m_gamePad->IsPress(enButtonLB1)) {
        m_defenceData.SetFlagDefense(true);
    }
    else {
        m_defenceData.SetFlagDefense(false);
    }

    // Debug: start
    if (m_playerNum == m_findGameData->GetOtherPlayerNum()) {
        m_defenceData.SetFlagDefense(true);
    }
    // Debug: end

    // Debug: Startボタン: ゲーム終了
    if (true == m_gamePad->IsTrigger(enButtonStart)) {
        //ゲームを終了
        exit(EXIT_SUCCESS);
    }



    // プレイヤーの移動
    Vector3 moveAmount = Vector3::Zero;

    // ガード中は処理をしない
    if (false == m_defenceData.GetFlagDefense()) {
        if (false == m_dashStatus.GetFlagDash()) {
            moveAmount = Move();
        }
        // ダッシュ
        else {
            moveAmount = DashMove();
        }
    }

    // スウェーの処理
    Vector2 swayMove = g_vec2Zero;
    if (m_gamePad->GetRStickXF() != 0.0f) {
        swayMove.x += m_gamePad->GetRStickXF();
    }
    if (m_gamePad->GetRStickYF() != 0.0f) {
        swayMove.y += m_gamePad->GetRStickYF();
    }

    // プレイヤーのモデルに位置情報などのステータス情報を渡す
    m_actor->AddStatus(moveAmount, swayMove);
}

const Vector3 Player::Move()
{
    // 現在の位置情報を保存
    //Vector3 oldPos = m_actor->GetPosition();

    // カメラの前方向を取得
    // ※カメラの前方向を参照する技を使うときに[cameraFront]が使えるかも
    Vector3 cameraFront = m_actor->GetPosition() - m_findPlayerCamera->GetPosition(m_playerNum);
    cameraFront.y = 0.0f;
    cameraFront.Normalize();

    // カメラの右方向
    Vector3 cameraRight = Cross(g_vec3AxisY, cameraFront);

    // キャラクターの移動速度
    float characterSpeed = 9.0f;

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

const Vector3 Player::DashMove()
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

void Player::UpdateAttack()
{
    //m_attackData.UpdateContinuousAttack();
    m_attackData.Update();

    // ダッシュ関連のUpdate
    m_dashStatus.DashUpdate();

    // ノックバック関連のUpdate
    m_squeezeStatus.SqueezeUpdate();

    // ダウン関連のUpdate
    m_downStatus.DownUpdate();

    // ガード関連のUpdate
    m_defenceData.Update();

    // 攻撃が当たったときの処理
    if (true == m_hitbox->UpdateCheckAttack()) {
        // 攻撃が当たった
        HitAttack();
    }
}

void Player::HitAttack()
{    
    // ダメージ処理
    if (false == m_otherPlayer->ReceiveDamage(m_attackData.GetPower())) {
        // ダメージを与えられてない
        return;
    }
}

void Player::AttackAnimationStart()
{
    // 攻撃時のアニメーションの再生を開始する関数
    // この関数は、良い感じの処理に変えたい
    // （Actorクラスのインスタンスで処理をするようにしたい）

    if (m_attackData.EnAttackType::enJub == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::jub);
        return;
    }
    if (m_attackData.EnAttackType::enUppercut == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::uppercut);
        return;
    }
    if (m_attackData.EnAttackType::enHook == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::hook);
        return;
    }
    if (m_attackData.EnAttackType::enBodyBlow == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::bodyBlow);
        return;
    }
    if (m_attackData.EnAttackType::enStraight == m_attackData.GetAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::straight);
        return;
    }
}