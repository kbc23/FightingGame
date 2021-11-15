#include "stdafx.h"
#include "st_attack_data.h"



namespace nsAttackData
{
    // ジャブ
    namespace nsJub
    {
        const int POWER = 10; // 攻撃力
        const int ATTACK_TIME_LIMIT = 16; // 攻撃時間
        const int DELAY_TIME_LIMIT = 4; // 攻撃までのディレイ
        const Vector3 RANGE = { 25.0f,25.0f,50.0f }; // 攻撃範囲
        const float POSITION_UP_Y = 100.0f; // 攻撃範囲のY座標の調整
        const float POSITION_UP_Z = 0.0f; // 攻撃範囲のZ座標の調整
        const int MAX_CONTINUOUS_ATTACK_COUNT = 3; // 連続攻撃の最大数
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // 攻撃による影響
    }
    
    // アッパーカット
    namespace nsUppercut
    {
        const int POWER = 15; // 攻撃力
        const int ATTACK_TIME_LIMIT = 15; // 攻撃時間
        const int DELAY_TIME_LIMIT = 7; // 攻撃までのディレイ
        const Vector3 RANGE = { 25.0f,40.0f,35.0f }; // 攻撃範囲
        const float POSITION_UP_Y = 90.0f; // 攻撃範囲のY座標の調整
        const float POSITION_UP_Z = 25.0f; // 攻撃範囲のZ座標の調整
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // 攻撃による影響
    }

    // フック
    namespace nsHook
    {
        const int POWER = 20; // 攻撃力
        const int ATTACK_TIME_LIMIT = 15; // 攻撃時間
        const int DELAY_TIME_LIMIT = 8; // 攻撃までのディレイ
        const Vector3 RANGE = { 50.0f,20.0f,30.0f }; // 攻撃範囲
        const float POSITION_UP_Y = 100.0f; // 攻撃範囲のY座標の調整
        const float POSITION_UP_Z = 25.0f; // 攻撃範囲のZ座標の調整
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // 攻撃による影響
    }

    // ボディブロー
    namespace nsBodyBlow
    {
        const int POWER = 25; // 攻撃力
        const int ATTACK_TIME_LIMIT = 21; // 攻撃時間
        const int DELAY_TIME_LIMIT = 19; // 攻撃までのディレイ
        const Vector3 RANGE = { 20.0f,20.0f,35.0f }; // 攻撃範囲
        const float POSITION_UP_Y = 75.0f; // 攻撃範囲のY座標の調整
        const float POSITION_UP_Z = 30.0f; // 攻撃範囲のZ座標の調整
        const int IMPACT_TYPE = StAttackData::EnImpactType::enSqueeze; // 攻撃による影響
    }

    // ストレート
    namespace nsStraight
    {
        const int POWER = 50; // 攻撃力
        const int ATTACK_TIME_LIMIT = 22; // 攻撃時間
        const int DELAY_TIME_LIMIT = 19; // 攻撃までのディレイ
        const Vector3 RANGE = { 25.0f,20.0f,85.0f }; // 攻撃範囲
        const float POSITION_UP_Y = 85.0f; // 攻撃範囲のY座標の調整
        const float POSITION_UP_Z = 0.0f; // 攻撃範囲のZ座標の調整
        const int IMPACT_TYPE = StAttackData::EnImpactType::enDown; // 攻撃による影響
    }
}



const bool StAttackData::SetAttackData(const int attackType)
{
    if (m_nextNGAttackType == attackType) {
        return false;
    }

    // ジャブ
    if (EnAttackType::enJub == attackType) {
        m_power = nsAttackData::nsJub::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsJub::ATTACK_TIME_LIMIT; // 攻撃時間
        m_delayTimeLimit = nsAttackData::nsJub::DELAY_TIME_LIMIT; // 攻撃までのディレイ
        m_range = nsAttackData::nsJub::RANGE; // 攻撃範囲
        m_positionUpY= nsAttackData::nsJub::POSITION_UP_Y; // 攻撃範囲のY座標の調整
        m_positionUpZ = nsAttackData::nsJub::POSITION_UP_Z; // 攻撃範囲のZ座標の調整
        m_attackType = EnAttackType::enJub; // 攻撃の種類
        m_impactType = nsAttackData::nsJub::IMPACT_TYPE; // 攻撃による影響
    }
    // アッパーカット
    else if (EnAttackType::enUppercut == attackType) {
        m_power = nsAttackData::nsUppercut::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsUppercut::ATTACK_TIME_LIMIT; // 攻撃時間
        m_delayTimeLimit = nsAttackData::nsUppercut::DELAY_TIME_LIMIT; // 攻撃までのディレイ
        m_range = nsAttackData::nsUppercut::RANGE; // 攻撃範囲
        m_positionUpY = nsAttackData::nsUppercut::POSITION_UP_Y; // 攻撃範囲のY座標の調整
        m_positionUpZ = nsAttackData::nsUppercut::POSITION_UP_Z; // 攻撃範囲のZ座標の調整
        m_attackType = EnAttackType::enUppercut; // 攻撃の種類
        m_impactType = nsAttackData::nsUppercut::IMPACT_TYPE; // 攻撃による影響
    }
    // フック
    else if (EnAttackType::enHook == attackType) {
        m_power = nsAttackData::nsHook::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsHook::ATTACK_TIME_LIMIT; // 攻撃時間
        m_delayTimeLimit = nsAttackData::nsHook::DELAY_TIME_LIMIT; // 攻撃までのディレイ
        m_range = nsAttackData::nsHook::RANGE; // 攻撃範囲
        m_positionUpY = nsAttackData::nsHook::POSITION_UP_Y; // 攻撃範囲のY座標の調整
        m_positionUpZ = nsAttackData::nsHook::POSITION_UP_Z; // 攻撃範囲のZ座標の調整
        m_attackType = EnAttackType::enHook; // 攻撃の種類
        m_impactType = nsAttackData::nsHook::IMPACT_TYPE; // 攻撃による影響
    }
    // ボディブロー
    else if (EnAttackType::enBodyBlow == attackType) {
        m_power = nsAttackData::nsBodyBlow::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsBodyBlow::ATTACK_TIME_LIMIT; // 攻撃時間
        m_delayTimeLimit = nsAttackData::nsBodyBlow::DELAY_TIME_LIMIT; // 攻撃までのディレイ
        m_range = nsAttackData::nsBodyBlow::RANGE; // 攻撃範囲
        m_positionUpY = nsAttackData::nsBodyBlow::POSITION_UP_Y; // 攻撃範囲のY座標の調整
        m_positionUpZ = nsAttackData::nsBodyBlow::POSITION_UP_Z; // 攻撃範囲のZ座標の調整
        m_attackType = EnAttackType::enBodyBlow; // 攻撃の種類
        m_impactType = nsAttackData::nsBodyBlow::IMPACT_TYPE; // 攻撃による影響
    }
    // ストレート
    else if (EnAttackType::enStraight == attackType) {
        m_power = nsAttackData::nsStraight::POWER; // 攻撃力
        m_attackTimeLimit = nsAttackData::nsStraight::ATTACK_TIME_LIMIT; // 攻撃時間
        m_delayTimeLimit = nsAttackData::nsStraight::DELAY_TIME_LIMIT; // 攻撃までのディレイ
        m_range = nsAttackData::nsStraight::RANGE; // 攻撃範囲
        m_positionUpY = nsAttackData::nsStraight::POSITION_UP_Y; // 攻撃範囲のY座標の調整
        m_positionUpZ = nsAttackData::nsStraight::POSITION_UP_Z; // 攻撃範囲のZ座標の調整
        m_attackType = EnAttackType::enStraight; // 攻撃の種類
        m_impactType = nsAttackData::nsStraight::IMPACT_TYPE; // 攻撃による影響
    }

    m_flagAttackNow = true; // 現在攻撃中か
    m_flagStartAttack = true; // 攻撃を開始したか
    m_flagNextAttackPossible = false; // 次の攻撃ができるか
    m_flagFinishDelay = false; // ディレイが終わったか

    // 連続攻撃に関する処理（連続攻撃５回目のとき）
    if (m_MAX_COUNT_CONTINUOUS_ATTACK <= m_countContinuousAttack) {
        m_impactType = EnImpactType::enDown;
    }

    // 連続攻撃中のジャブが３回目のとき
    if (m_attackType == EnAttackType::enJub && m_MAX_CONTINUE_JUB_ATTACK <= m_continueJubAttack) {
        m_impactType = EnImpactType::enDown;
    }

    m_nextNGAttackType = EnAttackType::enNotAttacking;

    return true;
}

const Vector3& StAttackData::CreateAttackPosition(const Vector3& playerPosition, const Quaternion& playerRotation)
{
    // 生成する攻撃範囲のポジション
    // 前方向に攻撃範囲の前方向の半分の値を追加する
    Vector3 attackRangePosition =
    {
        playerPosition.x,
        playerPosition.y,
        // 攻撃範囲の前方向の半分の値と指定した値だけZ座標をずらす
        playerPosition.z - m_range.z / 2 - 20.0f - m_positionUpZ
    };
    // キャラクターのポジション
    Vector3 local_playerPosition = playerPosition;

    // 攻撃範囲のポジションからキャラクターのポジションのベクトルを取得
    Vector3 toPos = attackRangePosition - local_playerPosition;
    // キャラクターのQuaternionを使ってベクトルをプレイヤーの前方向に回転させる
    playerRotation.Apply(toPos);

    // 上記で取得した情報から、攻撃範囲を生成するポジションを取得
    attackRangePosition = local_playerPosition - toPos;

    // 指定した値だけY座標を上昇する
    attackRangePosition.y = m_positionUpY;

    // 攻撃判定を作成した判定にする
    m_flagCreateAttackRange = true;

    return attackRangePosition;
}

const bool StAttackData::UpdateFinish()
{
    ++m_attackTime;

    if (m_attackTimeLimit <= m_attackTime) {
        return true;
    }

    return false;
}

void StAttackData::UpdateContinuousAttack()
{
    if (false == m_flagContinuousAttack) {
        return;
    }

    ++m_continuousAttackGraceTime;

    if (m_CONTINUOUS_ATTACK_GRACE_TIME_LIMIT <= m_continuousAttackGraceTime) {
        m_continuousAttackGraceTime = 0;
        m_countContinuousAttack = 0;
        m_flagContinuousAttack = false;
        m_continueJubAttack = 0;
        m_nextNGAttackType = EnAttackType::enNotAttacking;
    }
}

const bool StAttackData::DelayAttack()
{
    if (true == m_flagFinishDelay) {
        return true;
    }

    ++m_delayTime;

    if (m_delayTimeLimit <= m_delayTime) {
        m_flagFinishDelay = true;

        return true;
    }

    return false;
}

void StAttackData::ResetAttackData()
{
    // 攻撃時のステータスの初期化
    m_power = 0; // 攻撃力　
    m_attackTime = 0; // 攻撃時間
    m_attackTimeLimit = 0; // 攻撃時間の上限
    m_delayTime = 0; // 攻撃までのディレイ
    m_delayTimeLimit = 0; // 攻撃までのディレイの上限
    m_flagFinishDelay = true; // ディレイが終わったか
    m_range = Vector3::Zero; // 範囲
    m_flagAlreadyAttacked = false; // 攻撃がもう当たっているか
    m_flagAttackNow = false;
    m_flagCreateAttackRange = false;
    m_impactType = EnImpactType::enNotImpact;
}

void StAttackData::HitCheckAttackData()
{
    if (EnAttackType::enJub == m_attackType) {
        m_flagNextAttackPossible = true;
        ++m_continueJubAttack;
    }

    if (EnAttackType::enHook == m_attackType) {
        m_flagNextAttackPossible = true;
        m_nextNGAttackType = EnAttackType::enHook;
    }

    if (EnAttackType::enUppercut == m_attackType) {
        m_flagNextAttackPossible = true;
        m_nextNGAttackType = EnAttackType::enUppercut;
    }

    if (EnAttackType::enBodyBlow == m_attackType) {
        m_flagNextAttackPossible = true;
        m_nextNGAttackType = EnAttackType::enBodyBlow;
    }

    // 連続攻撃に関する処理
    ++m_countContinuousAttack;
    m_continuousAttackGraceTime = 0;
    m_flagContinuousAttack = true;
}