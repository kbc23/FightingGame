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

void Actor::Init(
    const char* filePath,
    const Vector3& initPos,
    float initRotAngle
)
{
    //アニメーションの設定
    //m_animationPlayer[AnimationEnum::enIdle].Load(
    //    m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enIdle).c_str()
    //);
    //m_animationPlayer[AnimationEnum::enJub].Load(
    //    m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enJub).c_str()
    //);
    //m_animationPlayer[AnimationEnum::enUppercut].Load(
    //    m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enUppercut).c_str()
    //);
    //m_animationPlayer[AnimationEnum::enHook].Load(
    //    m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enHook).c_str()
    //);
    //m_animationPlayer[AnimationEnum::enBodyBlow].Load(
    //    m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enBodyBlow).c_str()
    //);
    //m_animationPlayer[AnimationEnum::enStraight].Load(
    //    m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enStraight).c_str()
    //);
    //m_animationPlayer[AnimationEnum::enCrouchingStart].Load(
    //    m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enCrouchingStart).c_str()
    //);
    //m_animationPlayer[AnimationEnum::enCrouching].Load(
    //    m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enCrouching).c_str()
    //);
    //m_animationPlayer[AnimationEnum::enCrouchingEnd].Load(
    //    m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enCrouchingEnd).c_str()
    //);
    //ループ再生をtrueにする
    m_animationPlayer[enIdle].SetLoopFlag(true);

    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    // キャラクターモデル
    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    //m_modelCharacter->Init(m_readCSVFileCharacterData.GetCharacterModelPath().c_str(),
    //    false, true, modelUpAxis::enModelUpAxisZ, m_animationPlayer, AnimationMax
    //);
    m_modelCharacter->PlayAnimation(enIdle); //アニメーションの再生

    // 初期位置の設定
    m_position = initPos;
    m_modelCharacter->SetPosition(m_position);
    // 初期回転の設定
    m_rotY = initRotAngle;
    m_rotation.SetRotationY(m_rotY);
    m_modelCharacter->SetRotation(m_rotation);
    // 初期拡大率の設定
    m_scale = { 2.0f,2.0f,2.0f };
    m_modelCharacter->SetScale(m_scale);
}

void Actor::DebugInit(const char* filePath, const Vector3& initPos, const float initRot)
{
    //アニメーションの設定
    m_animationPlayer[AnimationEnum::enIdle].Load(
        m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enIdle).c_str()
    );
    m_animationPlayer[AnimationEnum::enJub].Load(
        m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enJub).c_str()
    );
    m_animationPlayer[AnimationEnum::enUppercut].Load(
        m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enUppercut).c_str()
    );
    m_animationPlayer[AnimationEnum::enHook].Load(
        m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enHook).c_str()
    );
    m_animationPlayer[AnimationEnum::enBodyBlow].Load(
        m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enBodyBlow).c_str()
    );
    m_animationPlayer[AnimationEnum::enStraight].Load(
        m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enStraight).c_str()
    );
    m_animationPlayer[AnimationEnum::enCrouchingStart].Load(
        m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enCrouchingStart).c_str()
    );
    m_animationPlayer[AnimationEnum::enCrouching].Load(
        m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enCrouching).c_str()
    );
    m_animationPlayer[AnimationEnum::enCrouchingEnd].Load(
        m_readCSVFileCharacterData.GetCharacterAnimationPath(AnimationEnum::enCrouchingEnd).c_str()
    );
    //ループ再生をtrueにする
    m_animationPlayer[AnimationEnum::enIdle].SetLoopFlag(true);

    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    // キャラクターモデル
    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(m_readCSVFileCharacterData.GetCharacterModelPath().c_str(),
        false, true, modelUpAxis::enModelUpAxisZ,
        m_animationPlayer, AnimationEnum::AnimationMax
    );
    m_modelCharacter->PlayAnimation(AnimationEnum::enIdle); //アニメーションの再生

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
    Turn(addMoveAmount);

    // スウェーのセット
    m_modelCharacter->SetSwayMove(swayMove);

    // しゃがみの処理
    CrouchingUpdate(swayMove.y);
}

void Actor::Turn(Vector3& addMoveAmount)
{
    if (fabsf(addMoveAmount.x) < 0.001f
        && fabsf(addMoveAmount.z) < 0.001f) {
        // m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
        // このフレームではキャラは移動していないので旋回する必要はない
        return;
    }
    // atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数
    // m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる
    // atan2を使用して、角度を求めている
    // これが回転角度になる
    float angle = atan2(addMoveAmount.x, addMoveAmount.z);
    // atanが返してくる角度はラジアン単位なので
    // SetRotationDegではなくSetRotationを使用する
    m_rotation.SetRotation(Vector3::AxisY, angle);
}

void Actor::AttackAnimation()
{
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