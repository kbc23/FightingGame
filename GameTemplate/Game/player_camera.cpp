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
    UpdatePlayerCamera();
    UpdateOtherPlayerCamera();
}

void PlayerCamera::UpdatePlayerCamera()
{
    //注視点はエネミーの座標にします
    m_cameraTarget = m_enemyPosition;
    //エネミーからプレイヤーに伸びるベクトルを求めます
    Vector3 pos = m_playerPosition - m_enemyPosition;
    //カメラの高さは一定にしたいので、y成分を0にします
    pos.y = 0.0f;
    //ベクトルを正規化します
    pos.Normalize();
    //スカラーを掛けます
    pos *= 200.0f;
    //カメラがどれだけプレイヤーの座標より高いかを設定します
    pos.y = 150.0f;
    //プレイヤーの座標に求めたベクトルを足して、カメラの座標とします
    m_cameraPosition = m_playerPosition + pos;

    m_cameraTarget.y = 75.0f;

    g_camera3D->SetPosition(m_cameraPosition); // カメラに位置の情報を渡す
    //g_camera3D->MoveRight(100.0f); // カメラの位置を右に少しずらす
    g_camera3D->SetTarget(m_cameraTarget); // カメラに注視点の情報を渡す
}

void PlayerCamera::UpdateOtherPlayerCamera()
{
    //注視点はエネミーの座標にします
    m_otherCameraTarget = m_playerPosition;
    //エネミーからプレイヤーに伸びるベクトルを求めます
    Vector3 pos = m_enemyPosition - m_playerPosition;
    //カメラの高さは一定にしたいので、y成分を0にします
    pos.y = 0.0f;
    //ベクトルを正規化します
    pos.Normalize();
    //スカラーを掛けます
    pos *= 400.0f;
    //カメラがどれだけプレイヤーの座標より高いかを設定します
    pos.y = 200.0f;
    //プレイヤーの座標に求めたベクトルを足して、カメラの座標とします
    m_otherCameraPosition = m_enemyPosition + pos;

}