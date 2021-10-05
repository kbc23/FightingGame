#pragma once

#include "HID/GamePad.h"
#include "FPSLimitter.h"

class GraphicsEngine;

class TkEngine {
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TkEngine();
	/// <summary>
	/// フレームの開始時に呼ばれる処理。
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// フレームの終了時に呼ばれる処理。
	/// </summary>
	void EndFrame();
	/// <summary>
	/// ゲームエンジンの初期化。
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。
	GameTime m_gameTime;							//ゲームタイム。
	


public:
	/// <summary>
	/// フレームレートモード
	/// </summary>
	enum EnFrameRateMode {
		enFrameRateMode_Fix,		// 固定フレームレートモード。
		enFrameRateMode_Variable,	// 可変フレームレートモード。
	};
	/// <summary>
	/// フレームレートに関する情報
	/// </summary>
	struct FrameRateInfo {
		EnFrameRateMode frameRateMode;	// フレームレートモード
		int maxFPS;						// 最大FPS
	};

	/// <summary>
	/// 現在のフレームレートに関する情報を取得
	/// </summary>
	/// <returns></returns>
	const FrameRateInfo& GetFrameRateInfo() const
	{
		return m_frameRateInfo;
	}
	/// <summary>
	/// フレームレートモードを設定する。
	/// </summary>
	/// <param name="frameRateMode">EnFrameRateModeを参照</param>
	/// <param name="maxFPS">最大FPS</param>
	void SetFrameRateMode(EnFrameRateMode frameRateMode, int maxFPS)
	{
		if (frameRateMode == enFrameRateMode_Fix) {
			// 固定フレームレート
			// 1フレームの経過時間の値を固定にする。
			m_gameTime.EnableFixedFrameDeltaTime(1.0f / maxFPS);
		}
		else {
			// 可変フレームレート
			m_gameTime.DisableFixedFrameDeltaTime();
		}
		m_fpsLimitter.SetMaxFPS(maxFPS);
		// フレームレートに関する情報を記憶。
		m_frameRateInfo.frameRateMode = frameRateMode;
		m_frameRateInfo.maxFPS = maxFPS;
	}

private:
	FrameRateInfo m_frameRateInfo = { enFrameRateMode_Variable , 60 };
	FPSLimitter m_fpsLimitter;						// FPSに制限をかける処理。



};

extern TkEngine* g_engine;	//TKエンジン。
extern GameTime* g_gameTime;