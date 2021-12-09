#include "stdafx.h"
#include "player_controller.h"

#include "st_player_status.h"

PlayerController::PlayerController()
{

}

PlayerController::~PlayerController()
{

}

bool PlayerController::Start()
{
    return true;
}

void PlayerController::Init(GamePad& gamePad, StPlayerStatus& playerStatus)
{
    m_gamePad = &gamePad;
    m_getStPlayerStatus = &playerStatus;
}

void PlayerController::Update()
{

}

void PlayerController::ControllerButton()
{
    // Aボタン
    if (false == m_getStPlayerStatus->NotAttack() && true == m_gamePad->IsTrigger(enButtonA)) {
        m_getStPlayerStatus->SetAttackData(enButtonA);
    }
    // Bボタン
    if (false == m_getStPlayerStatus->NotAttack() && true == m_gamePad->IsTrigger(enButtonB)) {
        m_getStPlayerStatus->SetAttackData(enButtonB);
    }
    // Xボタン
    if (false == m_getStPlayerStatus->NotAttack() && true == m_gamePad->IsTrigger(enButtonX)) {
        m_getStPlayerStatus->SetAttackData(enButtonX);
    }
    // Yボタン
    if (false == m_getStPlayerStatus->NotAttack() && true == m_gamePad->IsTrigger(enButtonY)) {
        m_getStPlayerStatus->SetAttackData(enButtonY);
    }
    // R2ボタン
    if (false == m_getStPlayerStatus->NotAttack() && true == m_gamePad->IsTrigger(enButtonRB2)) {
        m_getStPlayerStatus->SetAttackData(enButtonRB2);
    }

    // R1ボタン: ダッシュ
    if (false == m_getStPlayerStatus->CheckNowDefence() && true == m_gamePad->IsTrigger(enButtonRB1)) {
        m_getStPlayerStatus->StartDash();
    }

    // L1ボタン: ガード
    if (false == m_getStPlayerStatus->CheckNowDash() && true == m_gamePad->IsPress(enButtonLB1)) {
        m_getStPlayerStatus->StartDefence();
    }
    else {
        m_getStPlayerStatus->EndDefence();
    }

    // Debug: start
    //if (m_playerNum == m_findGameData->GetOtherPlayerNum()) {
    //    m_defenceData.SetFlagDefense(true);
    //}
    // Debug: end

    // Debug: Startボタン: ゲーム終了
    if (true == m_gamePad->IsTrigger(enButtonStart)) {
        //ゲームを終了
        exit(EXIT_SUCCESS);
    }
    // Debug: end
}

const Vector3 PlayerController::ControllerLStick()
{
    // プレイヤーの移動
    Vector3 moveAmount = Vector3::Zero;

    moveAmount.x = m_gamePad->GetLStickXF();
    moveAmount.y = 0.0f;
    moveAmount.z = m_gamePad->GetLStickYF();

    return moveAmount;
}

const Vector2 PlayerController::ControllerRStick()
{
    // スウェーの処理
    Vector2 swayMove = g_vec2Zero;

    swayMove.x = m_gamePad->GetRStickXF();
    swayMove.y = m_gamePad->GetRStickYF();

    return swayMove;
}
