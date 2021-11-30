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
		// �L�������I�΂ꂽ�̃I�����C���}�b�`���X�^�[�g����B
		m_onlineTwoPlayerMatchEngine = new NetWork;
		m_onlineTwoPlayerMatchEngine->Init(
			&m_charaNo,
			sizeof(int),
			[&](void* pData, int size) { OnAllPlayerJoined(pData, size); },
			[&]() { OnAllPlayerStartGame(); },
			[&]() { OnError(); }
		);
		// ���ׂẴv���C���[�������̂�҂B
		//m_fontRender.SetText(L"WaitAllPlayerJoined!!");
		m_step = EnStep::enStep_WaitAllPlayerJoined;
		//}
	}break;
		break;
	case EnStep::enStep_WaitAllPlayerJoined:
		// �S�Ẵv���C���[�����[���ɃW���C������̂�҂��Ă���B
		break;
	case EnStep::enStep_WaitAllPlayerStartGame:
		// �S�Ẵv���C���[���Q�[���J�n�\�ɂȂ�̂�҂��Ă���B
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
// �Q�[�����̏���
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
	// �Q�[���̏I�������̃e�X�g
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
		//�Q�[�����I��
		exit(EXIT_SUCCESS);
	}
	// Debug end
}

//////////////////////////////
// �Q�[���I������
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
// �l�b�g���[�N�֘A�̊֐�
////////////////////////////////////////////////////////////

void Game::OnAllPlayerJoined(void* pData, int size)
{
	// �����Ƒ���̃v���C���[�ԍ����擾
	m_playerNum = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
	m_otherPlayerNum = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();

	// ���ׂẴv���C���[��������
	m_player[m_playerNum] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);
	m_player[m_otherPlayerNum] = NewGO<Player>(igo::EnPriority::normal, igo::className::OTHER_PLAYER);

	m_playerCamera = NewGO<PlayerCamera>(igo::EnPriority::normal, igo::className::PLAYER_CAMERA);

	const Vector3 pos[] = {
	{100.0f, 0.0f, 0.0f},		// 1P�̏������W
	{-100.0f, 0.0f, 0.0f},		// 2P�̏������W
	};
	float rotAngle[] = {
		-90.0f,
		90.0f
	};

	m_gameData = NewGO<GameData>(igo::EnPriority::normal, igo::className::GAME_DATA);
	m_gameData->SetPlayerNumAndOtherPlayerNum(m_playerNum, m_otherPlayerNum);

	m_playerCamera->SetPlayerNum(m_playerNum, m_otherPlayerNum);

	// �����̃v���C���[����������
	m_player[m_playerNum]->Init(
		m_onlineTwoPlayerMatchEngine->GetGamePad(m_playerNum),
		pos[m_playerNum],
		rotAngle[m_playerNum],
		m_playerNum,
		m_player[m_otherPlayerNum]
	);
	// ����̃v���C���[����������
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

	// �l�b�g���[�N�̏���

	// ���[�h���I����ăQ�[���J�n�\�ɂȂ������Ƃ�ʒm����B
	m_onlineTwoPlayerMatchEngine->NotifyPossibleStartPlayGame();
	// �ق��̃v���C���[���Q�[���J�n�\�ɂȂ�܂ő҂B
	m_step = EnStep::enStep_WaitAllPlayerStartGame;

	//m_fontRender.SetText(L"WaitAllPlayerStartGame!!");
}

void Game::OnAllPlayerStartGame()
{
	// ���ׂẴv���C���[���Q�[���J�n�\�ɂȂ����̂ŁA�Q�[���X�^�[�g�I
	//m_fontRender.SetText(L"InGame!!");
	m_step = EnStep::enStep_InGame;

}

void Game::OnError()
{
	MessageBoxA(nullptr, "�ʐM�G���[���N���܂����B", "�G���[", MB_OK);
	m_step = EnStep::enStep_Error;
}