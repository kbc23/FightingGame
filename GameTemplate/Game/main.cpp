#include "stdafx.h"
#include "system/system.h"
#include "shadow.h"
#include "shadow_map.h"
#include "shadow_light_camera.h"
#include "game.h"


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

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	ShadowMap::CreateInstance();

	ShadowMap::GetInstance()->CreateShadowMap();

	ShadowLightCamera::CreateInstance();

	ShadowLightCamera::GetInstance()->CreateShadowLightCamera();

	Game* game = NewGO<Game>(0);
	
	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();

		GameObjectManager::GetInstance()->ExecuteUpdate(); //���t���[����Update	

		// �V���h�E�}�b�v�Ƀ����_�����O
		// �����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����
		renderContext.WaitUntilToPossibleSetRenderTarget(ShadowMap::GetInstance()->GetShadowMap());
		renderContext.SetRenderTargetAndViewport(ShadowMap::GetInstance()->GetShadowMap());
		renderContext.ClearRenderTargetView(ShadowMap::GetInstance()->GetShadowMap());

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

		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////

		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();

	ShadowMap::DeleteInstance();

	ShadowLightCamera::DeleteInstance();

	return 0;
}

