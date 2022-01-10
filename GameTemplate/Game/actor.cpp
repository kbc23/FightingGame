#include "stdafx.h"
#include "actor.h"



namespace // constant
{
    const float PLAYER_COLLIDER_RADIUS = 20.0f;
    const float PLAYER_COLLIDER_HEIGHT = 100.0f;

}



Actor::Actor()
{
    m_readCSVFileCharacterData.ReadCSVFileData("Assets/Data/data.csv");
}

Actor::~Actor()
{

}

bool Actor::Start()
{
    return true;
}

////////////////////////////////////////////////////////////
// 初期化
////////////////////////////////////////////////////////////

void Actor::Init(const Vector3& initPos, float initRotAngle)
{
    //アニメーションの設定
    if (false == AnimationInit()) {
        MessageBoxA(nullptr, "キャラクターのアニメーションの初期化に失敗したので終了します", "エラー", MB_OK);
        //ゲームを終了
        exit(EXIT_SUCCESS);
    }

    // キャラクターモデル
    CharacterInit(initPos, initRotAngle);
}

const bool Actor::AnimationInit()
{
    for (int num = 0; AnimationEnum::AnimationMax > num; ++num) {
        if (false == m_animationPlayer[num].Load(
            m_readCSVFileCharacterData.GetCharacterAnimationPath(num).c_str())
            )
        {
            return false;
        }
    }

    //ループ再生をtrueにする
    m_animationPlayer[AnimationEnum::enIdle].SetLoopFlag(true);

    return true;
}

void Actor::CharacterInit(const Vector3& initPos, const float initRot)
{
    // キャラクターモデル
    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(m_readCSVFileCharacterData.GetCharacterModelPath().c_str(),
        false, true, modelUpAxis::enModelUpAxisZ,
        m_animationPlayer, AnimationEnum::AnimationMax
    );

    //アニメーションの再生
    m_modelCharacter->PlayAnimation(AnimationEnum::enIdle);

    // 初期位置の設定
    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    // 初期回転の設定
    m_rotY = initRot;
    m_rotation.SetRotationY(m_rotY);
    m_modelCharacter->SetRotation(m_rotation);
    // 初期拡大率の設定
    m_scale = { 2.0f,2.0f,2.0f };
    m_modelCharacter->SetScale(m_scale);
}

void Actor::Update()
{
    KnockDownAnimation();
    // 攻撃アニメーション関連
    AttackAnimation();
    // モデルに位置情報などのデータを渡す
    SetModelStatus();
}

void Actor::AddStatus(Vector3& addMoveAmount, const Vector2& swayMove)
{
    // キャラクターの位置を決定
    m_position += addMoveAmount;
    // キャラクターの向きを決定
    PlayerViewingDirection();

    // スウェーのセット
    m_modelCharacter->SetSwayMove(swayMove);

    // しゃがみの処理
    CrouchingUpdate(swayMove.y);
}

void Actor::PlayerViewingDirection()
{
    if (m_getOtherActor == nullptr) {
        return;
    }

    Vector3 viewingDirection = m_getOtherActor->GetPosition() - m_position;

    // atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数
    // m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる
    // atan2を使用して、角度を求めている
    // これが回転角度になる
    float viewingAngle = atan2(viewingDirection.x, viewingDirection.z);
    // atanが返してくる角度はラジアン単位なので
    // SetRotationDegではなくSetRotationを使用する
    m_rotation.SetRotation(Vector3::AxisY, viewingAngle);
}

void Actor::AttackAnimation()
{
    if (true == m_hp0) {
        return;
    }

    // 攻撃のアニメーション中ではない場合
    if (false == m_flagAttackAnimation) {
        return;
    }

    // アニメーションが再生されていない（攻撃のアニメーションの再生が終わった）
    if (false == m_modelCharacter->IsPlayingAnimation()) {
        m_modelCharacter->PlayAnimation(AnimationEnum::enIdle); // 通常時のアニメーションにする
        m_flagAttackAnimation = false;
    }
}

void Actor::SetModelStatus()
{
    // 位置・回転・拡大情報を渡す
    m_modelCharacter->SetPosition(m_position);
    m_modelCharacter->SetRotation(m_rotation);
    m_modelCharacter->SetScale(m_scale);
}

void Actor::CrouchingUpdate(const float swayMoveY)
{
    // 前方向に入力
    if (0.5f <= swayMoveY) {
        // しゃがむ
        CrouchingStart();
    }
    // 前方向に入力していない
    else if (0.5f > swayMoveY) {
        // 起き上がる
        CrouchingEnd();
    }

    // しゃがみ中の処理
    Crouching();
}

void Actor::CrouchingStart()
{
    // しゃがむ
    if (EnCrouchingStatus::enNotCrouching == m_crouchingStatus) {
        m_crouchingStatus = EnCrouchingStatus::enStart;
        m_modelCharacter->PlayAnimation(AnimationEnum::enCrouchingStart);
    }
}

void Actor::Crouching()
{
    // しゃがみ開始
    if (EnCrouchingStatus::enStart == m_crouchingStatus) {
        // アニメーションが終わる
        if (false == m_modelCharacter->IsPlayingAnimation()) {
            // しゃがみ中の状態にし、アニメーションを再生
            m_crouchingStatus = EnCrouchingStatus::enCrouching;
            m_modelCharacter->PlayAnimation(AnimationEnum::enCrouching);
        }
    }
    // しゃがみ中
    else if (EnCrouchingStatus::enCrouching == m_crouchingStatus) {
        // アニメーションが終わる
        if (false == m_modelCharacter->IsPlayingAnimation()) {
            // 起き上がる状態にし、アニメーションを再生
            m_crouchingStatus = EnCrouchingStatus::enEnd;
            m_modelCharacter->PlayAnimation(AnimationEnum::enCrouchingEnd);
        }
    }
    // 起き上がり中
    else if (EnCrouchingStatus::enEnd == m_crouchingStatus) {
        // アニメーションが終わる
        if (false == m_modelCharacter->IsPlayingAnimation()) {
            // 通常状態にし、アニメーションを再生
            m_crouchingStatus = EnCrouchingStatus::enNotCrouching;
            m_modelCharacter->PlayAnimation(AnimationEnum::enIdle);
        }
    }
}

void Actor::CrouchingEnd()
{
    // しゃがみ中
    if (EnCrouchingStatus::enCrouching == m_crouchingStatus) {
        // 起き上がる状態にし、アニメーションを再生
        m_crouchingStatus = EnCrouchingStatus::enEnd;
        m_modelCharacter->PlayAnimation(AnimationEnum::enCrouchingEnd);
    }
}