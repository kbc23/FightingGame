#include "stdafx.h"
#include "game.h"

#include "constant.h"
#include "my_debug.h"
#include "actor.h"
#include "player.h"
#include "player_camera.h"

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
		// インゲーム
		//for (int i = 0; i < 2; i++) {
		//	const Vector3& actorPos = m_actor[i]->GetPosition();
		//	wchar_t text[256];
		//	swprintf_s(text, L"1P x : %f, y : %f, z : %f\n", actorPos.x, actorPos.y, actorPos.z);
		//	m_positionRender[i].SetText(text);
		//}
		int playerNo = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
		int otherPlayerNo = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();
		//if (m_actor[playerNo]->IsDowned()) {
		//	MessageBoxA(nullptr, "あなたの負け", "結果", MB_OK);
		//	ReturnCharacterSelect();
		//}
		//if (m_actor[otherPlayerNo]->IsDowned()) {
		//	MessageBoxA(nullptr, "あなたの勝ち", "結果", MB_OK);
		//	ReturnCharacterSelect();
		//}

		m_playerCamera->SetPlayerPosition(m_player[m_playerNum]->GetPosition());
		m_playerCamera->SetEnemyPosition(m_player[m_otherPlayerNum]->GetPosition());
	}break;
	case EnStep::enStep_Error:
		//ReturnCharacterSelect();
		m_step = EnStep::enStep_InitNetWork;
		break;
	}
}



////////////////////////////////////////////////////////////
// ネットワーク関連の関数
////////////////////////////////////////////////////////////

void Game::OnAllPlayerJoined(void* pData, int size)
{
	// すべてのプレイヤーが揃った
	m_player[0] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);
	m_player[1] = NewGO<Player>(igo::EnPriority::normal, igo::className::OTHER_PLAYER);

	m_playerCamera = NewGO<PlayerCamera>(igo::EnPriority::normal, igo::className::PLAYER_CAMERA);

	const Vector3 pos[] = {
	{100.0f, 0.0f, 0.0f},		// 1Pの初期座標
	{-100.0f, 0.0f, 0.0f},		// 2Pの初期座標
	};
	float rotAngle[] = {
		-90.0f,
		90.0f
	};
	// 自分と相手のプレイヤー番号を取得
	m_playerNum = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
	m_otherPlayerNum = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();

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