#include "stdafx.h"
#include "player_camera.h"

PlayerCamera::PlayerCamera()
{

}

PlayerCamera::~PlayerCamera()
{

}

bool PlayerCamera::Start()
{
    return true;
}

void PlayerCamera::Update()
{
    //注視点はエネミーの座標にします。
    m_target = m_enemyPosition;
    //エネミーからプレイヤーに伸びるベクトルを求めます。
    Vector3 pos = m_playerPosition - m_enemyPosition;
    //カメラの高さは一定にしたいので、y成分を0にします。
    pos.y = 0.0f;
    //ベクトルを正規化します。
    pos.Normalize();
    //スカラーを掛けます
    pos *= 500.0f;
    //カメラがどれだけプレイヤーの座標より高いかを設定します。
    pos.y = 300.0f;
    //プレイヤーの座標に求めたベクトルを足して、カメラの座標とします。
    m_position = m_playerPosition + pos;

    g_camera3D->SetTarget(m_target);
    g_camera3D->SetPosition(m_position);
}