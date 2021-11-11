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

    g_camera3D->SetTarget(m_cameraTarget); // カメラに注視点の情報を渡す
    g_camera3D->SetPosition(m_cameraPosition); // カメラに位置情報を渡す

    //g_camera3D->MoveForward(-100.0f);
    g_camera3D->MoveRight(50.0f);
    g_camera3D->SetTarget(m_cameraTarget); // カメラに注視点の情報を渡す


    // ここから下はテストで追加

    m_onelineCameraPosition = m_cameraPosition;
    return;

    m_playerRotation.AddRotationY(0.5f);

    Quaternion testRotation = g_quatIdentity;

    pos.y = 0;

    testRotation.SetRotation(g_vec3Zero, pos);

    //testRotation.AddRotationY(0.0f);

    m_cameraPosition.y = 0.0f;

    // 攻撃範囲のポジションからキャラクターのポジションのベクトルを取得
    Vector3 toPos = m_cameraPosition - m_playerPosition;
    // キャラクターのQuaternionを使ってベクトルをプレイヤーの前方向に回転させる
    testRotation.Apply(toPos);

    // 上記で取得した情報から、攻撃範囲を生成するポジションを取得
    m_cameraPosition = m_playerPosition - toPos;

    //カメラがどれだけプレイヤーの座標より高いかを設定します
    m_cameraPosition.y = 200.0f;

    g_camera3D->SetPosition(m_cameraPosition); // カメラに位置情報を渡す
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