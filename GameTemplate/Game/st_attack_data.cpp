#include "stdafx.h"
#include "st_attack_data.h"



namespace nsAttackData
{
    ////////////////////////////////////////////////////////////
    // 通常攻撃
    ////////////////////////////////////////////////////////////

    namespace nsNormalAttack
    {
        const int POWER = 50; // 攻撃力
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



void StAttackData::SetAttackData(const int attackType)
{
    // 通常攻撃
    if (EnAttackType::enNormal == attackType) {
        m_power = nsAttackData::nsNormalAttack::POWER;
        m_attackTimeLimit = nsAttackData::nsNormalAttack::ATTACK_TIME_LIMIT;
        m_range = nsAttackData::nsNormalAttack::RANGE;
        m_positionUpY = nsAttackData::nsNormalAttack::POSITION_UP_Y;
        m_flagAttackNow = true;
        m_attackType = EnAttackType::enNormal;
    }
    // サブ攻撃
    else if (EnAttackType::enSub == attackType) {
        m_power = nsAttackData::nsSubAttack::POWER;
        m_attackTimeLimit = nsAttackData::nsSubAttack::ATTACK_TIME_LIMIT;
        m_delayTimeLimit = nsAttackData::nsSubAttack::DELAY_TIME_LIMIT;
        m_flagFinishDelay = false;
        m_range = nsAttackData::nsSubAttack::RANGE;
        m_positionUpY = nsAttackData::nsSubAttack::POSITION_UP_Y;
        m_flagAttackNow = true;
        m_attackType = EnAttackType::enSub;
    }
}

const Vector3& StAttackData::CreateAttackPosition(const Vector3& playerPosition, const Quaternion& playerRotation)
{
    // 生成する攻撃範囲のポジション
    // 前方向に攻撃範囲の前方向の半分の値を追加する
    Vector3 attackRangePosition =
    {
        playerPosition.x,
        playerPosition.y,
        playerPosition.z - m_range.z / 2 - 20.0f // カメラの前方向に攻撃範囲の前方向の半分の値を追加
    };
    // キャラクターのポジション
    Vector3 local_playerPosition = playerPosition;

    // 攻撃範囲のポジションからキャラクターのポジションのベクトルを取得
    Vector3 toPos = attackRangePosition - local_playerPosition;
    // キャラクターのQuaternionを使ってベクトルをプレイヤーの前方向に回転させる
    playerRotation.Apply(toPos);

    // 上記で取得した情報から、攻撃範囲を生成するポジションを取得
    local_playerPosition = local_playerPosition - toPos;

    // 指定した値だけY座標を上昇する
    local_playerPosition.y = m_positionUpY;

    // 攻撃判定を作成した判定にする
    m_flagCreateAttackRange = true;

    return local_playerPosition;
}

const bool StAttackData::UpdateFinish()
{
    ++m_attackTime;

    if (m_attackTimeLimit <= m_attackTime) {
        return true;
    }

    return false;
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
    m_power = 0;
    m_attackTime = 0;
    m_attackTimeLimit = 0;
    m_delayTime = 0;
    m_delayTimeLimit = 0;
    m_flagFinishDelay = true;
    m_range = Vector3::Zero;
    m_positionUpY = 0.0f;
    m_flagAlreadyAttacked = false;
    m_flagAttackNow = false;
    m_flagCreateAttackRange = false;
}
