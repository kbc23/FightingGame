#include "stdafx.h"
#include "system/system.h"
#include "shadow.h"
#include "shadow_map.h"
#include "shadow_light_camera.h"
#include "game.h"


///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	ShadowMap::CreateInstance();

	ShadowMap::GetInstance()->CreateShadowMap();

	ShadowLightCamera::CreateInstance();

	ShadowLightCamera::GetInstance()->CreateShadowLightCamera();

	Game* game = NewGO<Game>(0);
	
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();

		GameObjectManager::GetInstance()->ExecuteUpdate(); //毎フレームのUpdate	

		// シャドウマップにレンダリング
		// レンダリングターゲットをシャドウマップに変更する
		renderContext.WaitUntilToPossibleSetRenderTarget(ShadowMap::GetInstance()->GetShadowMap());
		renderContext.SetRenderTargetAndViewport(ShadowMap::GetInstance()->GetShadowMap());
		renderContext.ClearRenderTargetView(ShadowMap::GetInstance()->GetShadowMap());

		GameObjectManager::GetInstance()->ExecuteShadowRender(renderContext);

		// 書き込み完了待ち
		renderContext.WaitUntilFinishDrawingToRenderTarget(ShadowMap::GetInstance()->GetShadowMap());

		// 通常レンダリング
		// レンダリングターゲットをフレームバッファーに戻す
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////

		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();

	ShadowMap::DeleteInstance();

	ShadowLightCamera::DeleteInstance();

	return 0;
}

