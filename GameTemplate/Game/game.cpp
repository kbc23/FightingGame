#include "stdafx.h"
#include "game.h"

#include "my_debug.h"
#include "actor.h"
#include "player.h"

Game::Game()
{
	m_modelCharacter = NewGO<ModelRender>(0);
	m_modelCharacter->Init("Assets/modelData/unityChan.tkm", false, true);

    m_modelStage = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelStage->Init("Assets/modelData/bg/bg.tkm", true);

	m_modelStage->SetPosition(m_position);

    //g_camera3D->SetTarget({ 0.0f,0.0f,0.0f });
    //g_camera3D->SetPosition({ 0.0f,800.0f,300.0f });
}

Game::~Game()
{
    DeleteGO(m_modelCharacter);
    DeleteGO(m_modelStage);
}

bool Game::Start()
{
    return true;
}

void Game::Update()
{
	//if (true == MyDebug::GetInstance()->GetDebugSoloMode()) {
	//	//１人でデバッグする
	//	SoloMode();

	//	return;
	//}

	return;


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
	



	// 下と上は別
	// すべてのプレイヤーが揃った
	// モデル関連のNewGO
	m_actor[0] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
	m_actor[1] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
	const Vector3 pos[] = {
		{100.0f, 0.0f, 0.0f},		// 1Pの初期座標
		{-100.0f, 0.0f, 0.0f},		// 2Pの初期座標
	};
	float rotAngle[] = {
		-90.0f,
		90.0f
	};

	// 自分のプレイヤー番号を取得
	int playerNo = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
	int otherPlayerNo = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();
	// 自分
	m_actor[playerNo]->Init(
		m_onlineTwoPlayerMatchEngine->GetGamePad(playerNo),
		"Assets/modelData/unityChan.tkm",
		pos[playerNo],
		rotAngle[playerNo],
		m_actor[otherPlayerNo]
	);
	// 対戦相手
	m_actor[otherPlayerNo]->Init(
		m_onlineTwoPlayerMatchEngine->GetGamePad(otherPlayerNo),
		"Assets/modelData/unityChan.tkm",
		pos[otherPlayerNo],
		rotAngle[otherPlayerNo],
		m_actor[playerNo]
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

////////////////////////////////////////////////////////////
// １人でデバッグする用
////////////////////////////////////////////////////////////

void Game::SoloMode()
{
  if (g_pad[0]->GetLStickXF() != 0.0f) {
      m_position.x -= g_pad[0]->GetLStickXF() * 5.0f;
  }
  if (g_pad[0]->GetLStickYF() != 0.0f) {
      m_position.z -= g_pad[0]->GetLStickYF() * 5.0f;
  }

  if (g_pad[0]->IsPress(enButtonA) == true) {
	  m_rotY += 0.01f;
	  m_rotation.SetRotationY(m_rotY);
  }
  if (g_pad[0]->IsPress(enButtonB) == true) {
	  m_rotY -= 0.01f;
	  m_rotation.SetRotationY(m_rotY);
  }

  m_modelCharacter->SetPosition(m_position);
  m_modelCharacter->SetRotation(m_rotation);
}