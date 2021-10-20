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
		// �C���Q�[��
		//for (int i = 0; i < 2; i++) {
		//	const Vector3& actorPos = m_actor[i]->GetPosition();
		//	wchar_t text[256];
		//	swprintf_s(text, L"1P x : %f, y : %f, z : %f\n", actorPos.x, actorPos.y, actorPos.z);
		//	m_positionRender[i].SetText(text);
		//}
		int playerNo = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
		int otherPlayerNo = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();
		//if (m_actor[playerNo]->IsDowned()) {
		//	MessageBoxA(nullptr, "���Ȃ��̕���", "����", MB_OK);
		//	ReturnCharacterSelect();
		//}
		//if (m_actor[otherPlayerNo]->IsDowned()) {
		//	MessageBoxA(nullptr, "���Ȃ��̏���", "����", MB_OK);
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
// �l�b�g���[�N�֘A�̊֐�
////////////////////////////////////////////////////////////

void Game::OnAllPlayerJoined(void* pData, int size)
{
	// ���ׂẴv���C���[��������
	m_player[0] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);
	m_player[1] = NewGO<Player>(igo::EnPriority::normal, igo::className::OTHER_PLAYER);

	m_playerCamera = NewGO<PlayerCamera>(igo::EnPriority::normal, igo::className::PLAYER_CAMERA);

	const Vector3 pos[] = {
	{100.0f, 0.0f, 0.0f},		// 1P�̏������W
	{-100.0f, 0.0f, 0.0f},		// 2P�̏������W
	};
	float rotAngle[] = {
		-90.0f,
		90.0f
	};
	// �����Ƒ���̃v���C���[�ԍ����擾
	m_playerNum = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
	m_otherPlayerNum = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();

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