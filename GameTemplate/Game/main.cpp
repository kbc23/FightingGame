#include "stdafx.h"
#include "system/system.h"
#include "shadow.h"
#include "shadow_map.h"
#include "shadow_light_camera.h"
#include "game.h"
#include "my_debug.h"



namespace debugModeStetus
{
	// �����̕ϐ��̏����l�Ńf�o�b�O���[�h�̐ݒ������
	bool flagDebugMode = true; // �f�o�b�O���[�h��
	bool flagSoloMode = true; // �\�����[�h�̃f�o�b�O���[�h��

	MyDebug* myDebug = nullptr;

	/**
	 * @brief �f�o�b�O���[�h�̐ݒ�
	 * @return �f�o�b�O���[�h��
	*/
	const bool StartDebugMode()
	{
		if (false == flagDebugMode) {
			return false;
		}

		// �f�o�b�O���[�h�̃C���X�^���X���쐬
		myDebug = NewGO<MyDebug>(igo::EnPriority::normal);

		// �f�o�b�O���[�h�ɂ���
		myDebug->OnDebugMode();

		if (true == flagSoloMode) {
			// �\�����[�h�ɂ���i�f�o�b�O���[�h�j
			myDebug->OnDebugSoloMode();
		}

		myDebug->Init();

		return true;
	}

	/**
	 * @brief �f�o�b�O���[�h�̏I������
	*/
	void FinishDebugMode()
	{
		if (false == flagDebugMode) {
			return;
		}

		// �f�o�b�O���[�h�̃C���X�^���X���폜
		DeleteGO(myDebug);
	}
}



namespace //constant
{
	const int NUMBER_OF_CONTROLLER = 4; //�R���g���[���[�̍ő�̐�
}



///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	//�R���g���[���[�̏�����
	for (int controllerNum = 0; controllerNum < NUMBER_OF_CONTROLLER; controllerNum++) {
		g_pad[controllerNum]->Init(controllerNum);
	}

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	ShadowMap::CreateInstance();

	ShadowMap::GetInstance()->CreateShadowMap();

	ShadowLightCamera::CreateInstance();

	ShadowLightCamera::GetInstance()->CreateShadowLightCamera();

	Game* game = nullptr;

	// �f�o�b�O���[�h�łȂ��Ƃ�
	if (false == debugModeStetus::StartDebugMode()) {
		game = NewGO<Game>(igo::EnPriority::normal, igo::className::GAME);
	}
	
	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();

		GameObjectManager::GetInstance()->ExecuteUpdate(); // ���t���[����Update

		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////

		// �V���h�E�}�b�v�Ƀ����_�����O
		// �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
		renderContext.WaitUntilToPossibleSetRenderTarget(ShadowMap::GetInstance()->GetShadowMap());
		renderContext.SetRenderTargetAndViewport(ShadowMap::GetInstance()->GetShadowMap());
		renderContext.ClearRenderTargetView(ShadowMap::GetInstance()->GetShadowMap());

		// �e�̃����_�����O
		GameObjectManager::GetInstance()->ExecuteShadowRender(renderContext);

		// �������݊����҂�
		renderContext.WaitUntilFinishDrawingToRenderTarget(ShadowMap::GetInstance()->GetShadowMap());

		// �ʏ탌���_�����O
		// �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�[�ɖ߂�
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		// ���f���̃����_�����O
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}

	DeleteGO(game);

	// �f�o�b�O���[�h���I������
	debugModeStetus::FinishDebugMode();

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();

	ShadowMap::DeleteInstance();

	ShadowLightCamera::DeleteInstance();

	return 0;
}

