#include "stdafx.h"
#include "game.h"

#include "constant.h"
#include "my_debug.h"
#include "player.h"
#include "player_camera.h"
#include "game_data.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
    return true;
}

void Game::Update()
{
	if (m_onlineTwoPlayerMatchEngine) {
		m_onlineTwoPlayerMatchEngine->Update();
	}
	switch (m_step) {
	case EnStep::enStep_InitNetWork: {

		//if (g_pad[0]->IsPress(enButtonA)) {
		//	m_charaNo = 0;
		//}
		//else if (g_pad[0]->IsPress(enButtonB)) {
		//	m_charaNo = 1;
		//}
		//if (m_charaNo != -1) {
		// キャラが選ばれたのオンラインマッチをスタートする。
		m_onlineTwoPlayerMatchEngine = new NetWork;
		m_onlineTwoPlayerMatchEngine->Init(
			&m_charaNo,
			sizeof(int),
			[&](void* pData, int size) { OnAllPlayerJoined(pData, size); },
			[&]() { OnAllPlayerStartGame(); },
			[&]() { OnError(); }
		);
		// すべてのプレイヤーが揃うのを待つ。
		//m_fontRender.SetText(L"WaitAllPlayerJoined!!");
		m_step = EnStep::enStep_WaitAllPlayerJoined;
		//}
	}break;
		break;
	case EnStep::enStep_WaitAllPlayerJoined:
		// 全てのプレイヤーがルームにジョインするのを待っている。
		break;
	case EnStep::enStep_WaitAllPlayerStartGame:
		// 全てのプレイヤーがゲーム開始可能になるのを待っている。
		break;
	case EnStep::enStep_InGame: {
		UpdateGame();

	}break;
	case EnStep::enStep_Error:
		//ReturnCharacterSelect();
		m_step = EnStep::enStep_InitNetWork;
		delete m_onlineTwoPlayerMatchEngine;
		break;
	}
}

////////////////////////////////////////////////////////////
// ゲーム中の処理
////////////////////////////////////////////////////////////

void Game::UpdateGame()
{
	switch (m_gameStatus) {
	case EnGameStatus::enInGame:
		InGame();
		break;
	case EnGameStatus::enFinishGame:
		FinishGame();
		break;
	default:

		break;
	}
}

void Game::InGame()
{
	m_playerCamera->SetPlayerPosition(m_player[m_playerNum]->GetPosition());
	m_playerCamera->SetEnemyPosition(m_player[m_otherPlayerNum]->GetPosition());

	CheckGameEnd();
}

void Game::FinishGame()
{
	// Debug start
	// ゲームの終了処理のテスト
	++m_debugCountGameEnd;

	if (30 == m_debugCountGameEnd) {
		if (EnWinOrLose::enWin == m_winOrLose[m_playerNum]) {
			m_fontWinOrLose->Init(L"WIN!");
		}
		else if (EnWinOrLose::enLose == m_winOrLose[m_playerNum]) {
			m_fontWinOrLose->Init(L"LOSE...");
		}
	}

	if (120 <= m_debugCountGameEnd) {
		//ゲームを終了
		exit(EXIT_SUCCESS);
	}
	// Debug end
}

//////////////////////////////
// ゲーム終了判定
//////////////////////////////

void Game::CheckGameEnd()
{
	if (false == CheckHp_0()) {
		return;
	}

	m_gameStatus = EnGameStatus::enFinishGame;

	m_player[m_playerNum]->SetFlagGameEndStopOperation(true);
	m_player[m_otherPlayerNum]->SetFlagGameEndStopOperation(true);
}

const bool Game::CheckHp_0()
{
	if (true == m_player[m_playerNum]->CheckHp_0()) {
		m_winOrLose[m_playerNum] = EnWinOrLose::enLose;
		m_winOrLose[m_otherPlayerNum] = EnWinOrLose::enWin;

		return true;
	}
	else if (true == m_player[m_otherPlayerNum]->CheckHp_0()) {
		m_winOrLose[m_playerNum] = EnWinOrLose::enWin;
		m_winOrLose[m_otherPlayerNum] = EnWinOrLose::enLose;

		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////
// ネットワーク関連の関数
////////////////////////////////////////////////////////////

void Game::OnAllPlayerJoined(void* pData, int size)
{
	// 自分と相手のプレイヤー番号を取得
	m_playerNum = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
	m_otherPlayerNum = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();

	// すべてのプレイヤーが揃った
	m_player[m_playerNum] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);
	m_player[m_otherPlayerNum] = NewGO<Player>(igo::EnPriority::normal, igo::className::OTHER_PLAYER);

	m_playerCamera = NewGO<PlayerCamera>(igo::EnPriority::normal, igo::className::PLAYER_CAMERA);

	const Vector3 pos[] = {
	{100.0f, 0.0f, 0.0f},		// 1Pの初期座標
	{-100.0f, 0.0f, 0.0f},		// 2Pの初期座標
	};
	float rotAngle[] = {
		-90.0f,
		90.0f
	};

	m_gameData = NewGO<GameData>(igo::EnPriority::normal, igo::className::GAME_DATA);
	m_gameData->SetPlayerNumAndOtherPlayerNum(m_playerNum, m_otherPlayerNum);

	m_playerCamera->SetPlayerNum(m_playerNum, m_otherPlayerNum);

	// 自分のプレイヤー情報を初期化
	m_player[m_playerNum]->Init(
		m_onlineTwoPlayerMatchEngine->GetGamePad(m_playerNum),
		pos[m_playerNum],
		rotAngle[m_playerNum],
		m_playerNum,
		m_player[m_otherPlayerNum]
	);
	// 相手のプレイヤー情報を初期化
	m_player[m_otherPlayerNum]->Init(
		m_onlineTwoPlayerMatchEngine->GetGamePad(m_otherPlayerNum),
		pos[m_otherPlayerNum],
		rotAngle[m_otherPlayerNum],
		m_otherPlayerNum,
		m_player[m_playerNum]
	);

	m_modelStage = NewGO<ModelRender>(igo::EnPriority::model);
	m_modelStage->Init("Assets/modelData/stage/stage.tkm", true);

	m_modelStage->SetPosition({ 0.0f,0.0f,0.0f });
	m_modelStage->SetScale({ 5.0f,1.0f,5.0f });

	m_fontWinOrLose = NewGO<FontRender>(igo::EnPriority::font);

	// ネットワークの処理

	// ロードが終わってゲーム開始可能になったことを通知する。
	m_onlineTwoPlayerMatchEngine->NotifyPossibleStartPlayGame();
	// ほかのプレイヤーがゲーム開始可能になるまで待つ。
	m_step = EnStep::enStep_WaitAllPlayerStartGame;

	//m_fontRender.SetText(L"WaitAllPlayerStartGame!!");
}

void Game::OnAllPlayerStartGame()
{
	// すべてのプレイヤーがゲーム開始可能になったので、ゲームスタート！
	//m_fontRender.SetText(L"InGame!!");
	m_step = EnStep::enStep_InGame;

}

void Game::OnError()
{
	MessageBoxA(nullptr, "通信エラーが起きました。", "エラー", MB_OK);
	m_step = EnStep::enStep_Error;
}