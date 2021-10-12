#include "stdafx.h"
#include "game.h"

#include "constant.h"
#include "my_debug.h"
#include "actor.h"
#include "player.h"
#include "player_camera.h"

Game::Game()
{
	//m_modelCharacter = NewGO<ModelRender>(0);
	//m_modelCharacter->Init("Assets/modelData/unityChan.tkm", false, true);

    //m_modelStage = NewGO<ModelRender>(igo::EnPriority::model);
    //m_modelStage->Init("Assets/modelData/bg/bg.tkm", true);

	//m_modelStage->SetPosition(m_position);

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

		m_playerCamera->SetPlayerPosition(m_player[m_playerNo]->GetPosition());
		m_playerCamera->SetEnemyPosition(m_player[m_otherPlayerNo]->GetPosition());
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
	m_player[1] = NewGO<Player>(igo::EnPriority::normal, igo::className::PLAYER);

	m_playerCamera = NewGO<PlayerCamera>(igo::EnPriority::normal);

	const Vector3 pos[] = {
	{100.0f, 0.0f, 0.0f},		// 1P�̏������W
	{-100.0f, 0.0f, 0.0f},		// 2P�̏������W
	};
	float rotAngle[] = {
		-90.0f,
		90.0f
	};
	// �����Ƒ���̃v���C���[�ԍ����擾
	m_playerNo = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
	m_otherPlayerNo = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();
	// �����̃v���C���[����������
	m_player[m_playerNo]->Init(
		m_onlineTwoPlayerMatchEngine->GetGamePad(m_playerNo),
		pos[m_playerNo],
		rotAngle[m_playerNo],
		m_player[m_otherPlayerNo]
	);
	// ����̃v���C���[����������
	m_player[m_otherPlayerNo]->Init(
		m_onlineTwoPlayerMatchEngine->GetGamePad(m_otherPlayerNo),
		pos[m_otherPlayerNo],
		rotAngle[m_otherPlayerNo],
		m_player[m_playerNo]
	);

	// ���[�h���I����ăQ�[���J�n�\�ɂȂ������Ƃ�ʒm����B
	m_onlineTwoPlayerMatchEngine->NotifyPossibleStartPlayGame();
	// �ق��̃v���C���[���Q�[���J�n�\�ɂȂ�܂ő҂B
	m_step = EnStep::enStep_WaitAllPlayerStartGame;

	//m_fontRender.SetText(L"WaitAllPlayerStartGame!!");

	return;

	// ���Ə�͕�
	// ���ׂẴv���C���[��������
	// ���f���֘A��NewGO
	//m_actor[0] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
	//m_actor[1] = NewGO<Actor>(igo::EnPriority::normal, igo::className::ACTOR);
	//const Vector3 pos2[] = {
	//	{100.0f, 0.0f, 0.0f},		// 1P�̏������W
	//	{-100.0f, 0.0f, 0.0f},		// 2P�̏������W
	//};
	//float rotAngle2[] = {
	//	-90.0f,
	//	90.0f
	//};

	//// �����̃v���C���[�ԍ����擾
	//int playerNo = m_onlineTwoPlayerMatchEngine->GetPlayerNo();
	//int otherPlayerNo = m_onlineTwoPlayerMatchEngine->GetOtherPlayerNo();
	//// ����
	//m_actor[playerNo]->Init(
	//	m_onlineTwoPlayerMatchEngine->GetGamePad(playerNo),
	//	"Assets/modelData/unityChan.tkm",
	//	pos2[playerNo],
	//	rotAngle2[playerNo],
	//	m_actor[otherPlayerNo]
	//);
	//// �ΐ푊��
	//m_actor[otherPlayerNo]->Init(
	//	m_onlineTwoPlayerMatchEngine->GetGamePad(otherPlayerNo),
	//	"Assets/modelData/unityChan.tkm",
	//	pos2[otherPlayerNo],
	//	rotAngle2[otherPlayerNo],
	//	m_actor[playerNo]
	//);


	//// ���[�h���I����ăQ�[���J�n�\�ɂȂ������Ƃ�ʒm����B
	//m_onlineTwoPlayerMatchEngine->NotifyPossibleStartPlayGame();
	//// �ق��̃v���C���[���Q�[���J�n�\�ɂȂ�܂ő҂B
	//m_step = EnStep::enStep_WaitAllPlayerStartGame;

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

////////////////////////////////////////////////////////////
// �P�l�Ńf�o�b�O����p
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