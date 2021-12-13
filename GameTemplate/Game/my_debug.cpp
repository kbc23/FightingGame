#include "stdafx.h"
#include "my_debug.h"

#include "constant.h"
#include "player.h"
#include "player_camera.h"
#include "game_data.h"

namespace // constant
{
    const enum EnPlayer
    {
        enPlayer,
        enOtherPlayer,
        EnPlayerNum
    };
}

void MyDebug::Init()
{
    m_player[enPlayer] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);
    m_player[enOtherPlayer] = NewGO<Player>(igo::EnPriority::normal, igo::className::OTHER_PLAYER);

    m_playerCamera = NewGO<PlayerCamera>(igo::EnPriority::normal, igo::className::PLAYER_CAMERA);

    const Vector3 pos[] =
    {
        {100.0f, 0.0f, 0.0f},   // 1Pの初期座標
        {-100.0f, 0.0f, 0.0f},  // 2Pの初期座標
    };
    float rotAngle[] = {
        -90.0f,
        0.0f
    };

    m_gameData = NewGO<GameData>(igo::EnPriority::normal, igo::className::GAME_DATA);
    m_gameData->SetPlayerNumAndOtherPlayerNum(enPlayer, enOtherPlayer);

    m_playerCamera->SetPlayerNum(enPlayer, enOtherPlayer);
    m_player[enPlayer]->Init(
        *g_pad[enPlayer], 
        pos[enPlayer], 
        rotAngle[enPlayer],
        enPlayer,  
        m_player[enOtherPlayer]
    );
    m_player[enOtherPlayer]->Init(
        *g_pad[enOtherPlayer], 
        pos[enOtherPlayer], 
        rotAngle[enOtherPlayer],
        enOtherPlayer, 
        m_player[enPlayer]
    );

    m_modelStage = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelStage->Init("Assets/modelData/stage/stage.tkm", true);

    m_modelStage->SetPosition({ 0.0f,0.0f,0.0f });

    m_modelStage->SetScale({ 5.0f,1.0f,5.0f });

    m_fontWinOrLose = NewGO<FontRender>(igo::EnPriority::font);
}

void MyDebug::Finish()
{
    DeleteGO(m_modelStage);
}

bool MyDebug::Start()
{
    return true;
}

void MyDebug::Update()
{
    switch (m_status) {
    case EnStatus::game:
        m_playerCamera->SetPlayerPosition(m_player[enPlayer]->GetPosition());
        m_playerCamera->SetEnemyPosition(m_player[enOtherPlayer]->GetPosition());

        // ゲーム終了判定
        GameEndUpdate();

        break;
    case EnStatus::finishGame:


        // Debug start
        // ゲームの終了処理のテスト
        ++m_debugCountGameEnd;

        if (30 == m_debugCountGameEnd) {
            if (EnWinOrLose::win == m_winOrLose[enPlayer]) {
                m_fontWinOrLose->Init(L"WIN!");
            }
            else if (EnWinOrLose::lose == m_winOrLose[enPlayer]) {
                m_fontWinOrLose->Init(L"LOSE...");
            }
        }

        if (120 <= m_debugCountGameEnd) {
            //ゲームを終了
            exit(EXIT_SUCCESS);
        }
        // Debug end
        break;
    default:

        break;
    }
}

////////////////////////////////////////////////////////////
// ゲーム終了関連
////////////////////////////////////////////////////////////

void MyDebug::GameEndUpdate()
{
    if (false == CheckGameEnd()) {
        return;
    }

    m_status = EnStatus::finishGame;

    m_player[enPlayer]->SetFlagGameEndStopOperation(true);
    m_player[enOtherPlayer]->SetFlagGameEndStopOperation(true);
}

bool MyDebug::CheckGameEnd()
{
    if (true == m_player[enPlayer]->CheckHp_0()) {
        m_winOrLose[enPlayer] = EnWinOrLose::lose;
        m_winOrLose[enOtherPlayer] = EnWinOrLose::win;

        return true;
    }
    else if (true == m_player[enOtherPlayer]->CheckHp_0()) {
        m_winOrLose[enPlayer] = EnWinOrLose::win;
        m_winOrLose[enOtherPlayer] = EnWinOrLose::lose;

        return true;
    }

    return false;
}