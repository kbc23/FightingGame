#pragma once

#include "HID/GamePad.h"
#include "FPSLimitter.h"

class GraphicsEngine;

class TkEngine {
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~TkEngine();
	/// <summary>
	/// �t���[���̊J�n���ɌĂ΂�鏈���B
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// �t���[���̏I�����ɌĂ΂�鏈���B
	/// </summary>
	void EndFrame();
	/// <summary>
	/// �Q�[���G���W���̏������B
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B
	GameTime m_gameTime;							//�Q�[���^�C���B
	


public:
	/// <summary>
	/// �t���[�����[�g���[�h
	/// </summary>
	enum EnFrameRateMode {
		enFrameRateMode_Fix,		// �Œ�t���[�����[�g���[�h�B
		enFrameRateMode_Variable,	// �σt���[�����[�g���[�h�B
	};
	/// <summary>
	/// �t���[�����[�g�Ɋւ�����
	/// </summary>
	struct FrameRateInfo {
		EnFrameRateMode frameRateMode;	// �t���[�����[�g���[�h
		int maxFPS;						// �ő�FPS
	};

	/// <summary>
	/// ���݂̃t���[�����[�g�Ɋւ�������擾
	/// </summary>
	/// <returns></returns>
	const FrameRateInfo& GetFrameRateInfo() const
	{
		return m_frameRateInfo;
	}
	/// <summary>
	/// �t���[�����[�g���[�h��ݒ肷��B
	/// </summary>
	/// <param name="frameRateMode">EnFrameRateMode���Q��</param>
	/// <param name="maxFPS">�ő�FPS</param>
	void SetFrameRateMode(EnFrameRateMode frameRateMode, int maxFPS)
	{
		if (frameRateMode == enFrameRateMode_Fix) {
			// �Œ�t���[�����[�g
			// 1�t���[���̌o�ߎ��Ԃ̒l���Œ�ɂ���B
			m_gameTime.EnableFixedFrameDeltaTime(1.0f / maxFPS);
		}
		else {
			// �σt���[�����[�g
			m_gameTime.DisableFixedFrameDeltaTime();
		}
		m_fpsLimitter.SetMaxFPS(maxFPS);
		// �t���[�����[�g�Ɋւ�������L���B
		m_frameRateInfo.frameRateMode = frameRateMode;
		m_frameRateInfo.maxFPS = maxFPS;
	}

private:
	FrameRateInfo m_frameRateInfo = { enFrameRateMode_Variable , 60 };
	FPSLimitter m_fpsLimitter;						// FPS�ɐ����������鏈���B



};

extern TkEngine* g_engine;	//TK�G���W���B
extern GameTime* g_gameTime;