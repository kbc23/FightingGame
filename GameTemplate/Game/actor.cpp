#include "stdafx.h"
#include "actor.h"



namespace // constant
{
    const float PLAYER_COLLIDER_RADIUS = 20.0f;
    const float PLAYER_COLLIDER_HEIGHT = 100.0f;

}



Actor::Actor()
{

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
    m_animationPlayer[enIdle].Load("Assets/modelData/model/idle.tka");
    m_animationPlayer[enJub].Load("Assets/modelData/model/Lead_Jab.tka");
    m_animationPlayer[enUppercut].Load("Assets/modelData/model/Uppercut.tka");
    m_animationPlayer[enHook].Load("Assets/modelData/model/hook.tka");
    m_animationPlayer[enBodyBlow].Load("Assets/modelData/model/Body_blow.tka");
    m_animationPlayer[enStraight].Load("Assets/modelData/model/straight.tka");
    //ループ再生をtrueにする
    m_animationPlayer[enIdle].SetLoopFlag(true);

    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    // キャラクターモデル
    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true, modelUpAxis::enModelUpAxisZ, m_animationPlayer, AnimationMax);
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
    m_animationPlayer[AnimationEnum::enIdle].Load("Assets/modelData/model/idle.tka");
    m_animationPlayer[AnimationEnum::enJub].Load("Assets/modelData/model/Lead_Jab.tka");
    m_animationPlayer[AnimationEnum::enUppercut].Load("Assets/modelData/model/Uppercut.tka");
    m_animationPlayer[AnimationEnum::enHook].Load("Assets/modelData/model/hook.tka");
    m_animationPlayer[AnimationEnum::enBodyBlow].Load("Assets/modelData/model/Body_blow.tka");
    m_animationPlayer[AnimationEnum::enStraight].Load("Assets/modelData/model/straight.tka");
    m_animationPlayer[AnimationEnum::enCrouchingStart].Load("Assets/modelData/model/Crouching_Start.tka");
    m_animationPlayer[AnimationEnum::enCrouching].Load("Assets/modelData/model/Crouching.tka");
    m_animationPlayer[AnimationEnum::enCrouchingEnd].Load("Assets/modelData/model/Crouching_End.tka");
    //ループ再生をtrueにする
    m_animationPlayer[AnimationEnum::enIdle].SetLoopFlag(true);

    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    // キャラクターモデル
    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->Init(filePath, false, true, modelUpAxis::enModelUpAxisZ,
                            m_animationPlayer, AnimationEnum::AnimationMax);
    m_modelCharacter->PlayAnimation(enIdle); //アニメーションの再生

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
        m_modelCharacter->PlayAnimation(enIdle); // 通常時のアニメーションにする
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