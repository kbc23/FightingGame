#include "stdafx.h"
#include "player.h"

#include "constant.h"
#include "player_camera.h"
#include "player_UI.h"
#include "game_data.h"
//#include "attack_move_bone.h"
#include "player_controller.h"

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
        "Assets/modelData/model/model.tkm",
        initPos,
        initRotAngle
    );

    // 当たり判定の初期化
    m_hitbox = NewGO<Hitbox>(igo::EnPriority::normal);
    m_hitbox->Init(*m_otherPlayer, *m_actor, m_playerStatus);

    // プレイヤーに関係するUIクラスの初期化
    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    m_playerController = NewGO<PlayerController>(igo::EnPriority::normal);
    m_playerController->Init(*m_gamePad, m_playerStatus);

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
    m_hitbox->Init(*m_otherPlayer, *m_actor, m_playerStatus);

    m_playerUI = NewGO<PlayerUI>(igo::EnPriority::normal);

    m_playerController = NewGO<PlayerController>(igo::EnPriority::normal);
    m_playerController->Init(*m_gamePad, m_playerStatus);

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
    //m_playerUI->UpdateDashUI(m_dashStatus.GetRemainingNumberOfTimes(), m_playerNum);
    //m_playerUI->UpdateKnockBackUI(m_squeezeStatus.GetFlagSqueeze(), m_playerNum);
    //m_playerUI->UpdateDownUI(m_downStatus.GetFlagDown(), m_playerNum);
    //m_playerUI->UpdateDefenseUI(m_defenceData.GetFlagDefense(), m_playerNum);
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

    if (true == m_playerStatus.NotController()) {
        return;
    }

    //////////////////////////////
    // ボタン入力
    //////////////////////////////

    m_playerController->ControllerButton();

    // プレイヤーの移動
    Vector3 moveAmount = m_playerController->ControllerLStick();

    // ガード中は処理をしない
    if (false == m_playerStatus.CheckNowDefence()) {
        if (false == m_playerStatus.CheckNowDash()) {
            moveAmount = Move(moveAmount);
        }
        // ダッシュ
        else {
            moveAmount = DashMove();
        }
    }
    else {
        moveAmount = Vector3::Zero;
    }

    // スウェーの処理
    Vector2 swayMove = m_playerController->ControllerRStick();

    // 攻撃時のアニメーションを再生
    if (true == m_playerStatus.CheckNowAttack()) {
        AttackAnimationStart();
    }
    // プレイヤーのモデルに位置情報などのステータス情報を渡す
    m_actor->AddStatus(moveAmount, swayMove);
}

const Vector3 Player::Move(const Vector3& moveAmountBeforeEditing)
{
    // カメラの前方向を取得
    Vector3 cameraFront = m_actor->GetPosition() - m_findPlayerCamera->GetPosition(m_playerNum);
    cameraFront.y = 0.0f;
    cameraFront.Normalize();

    // カメラの右方向
    Vector3 cameraRight = Cross(g_vec3AxisY, cameraFront);

    // キャラクターの移動速度
    const float CHARACTER_SPEED = 9.0f;

    Vector3 pospos = moveAmountBeforeEditing * CHARACTER_SPEED;

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
    toPos.z *= characterSpeed;
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
    // プレイヤーのステータス関係の更新処理
    m_playerStatus.StatusUpdate();

    // 攻撃が当たったときの処理
    if (true == m_hitbox->UpdateCheckAttack()) {
        // 攻撃が当たった
        HitAttack();
    }
}

void Player::HitAttack()
{    
    // ダメージ処理
    if (false == m_otherPlayer->ReceiveDamage(m_playerStatus.GetAttackPower())) {
        // ダメージを与えられてない
        return;
    }
}

void Player::AttackAnimationStart()
{
    // 攻撃時のアニメーションの再生を開始する関数
    // この関数は、良い感じの処理に変えたい
    // （Actorクラスのインスタンスで処理をするようにしたい）

    if (StAttackData::EnAttackType::enJub == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enJub);
        return;
    }
    if (StAttackData::EnAttackType::enUppercut == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enUppercut);
        return;
    }
    if (StAttackData::EnAttackType::enHook == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enHook);
        return;
    }
    if (StAttackData::EnAttackType::enBodyBlow == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enBodyBlow);
        return;
    }
    if (StAttackData::EnAttackType::enStraight == m_playerStatus.GetNowAttackType()) {
        m_actor->SetAttackAnimation(m_actor->AnimationEnum::enStraight);
        return;
    }
}