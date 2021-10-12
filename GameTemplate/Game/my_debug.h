#pragma once

class Player;
class PlayerCamera;

class MyDebug : public IGameObject
{
public:
	void Init();
	void Finish();
	bool Start() override;
	void Update() override;


private:
	void SoloMode();


public:
	/**
	 * @brief �f�o�b�O���[�h��
	 * @return �f�o�b�O���[�h���ǂ���
	*/
	const bool GetDebugMode()
	{
		return m_debugMode;
	}
	/**
	 * @brief �f�o�b�O�̃\�����[�h��
	 * @return �f�o�b�O�̃\�����[�h���ǂ���
	*/
	const bool GetDebugSoloMode()
	{
		if (false == m_debugMode) {
			return false;
		}

		return m_debugSoloMode;
	}



public:
	/**
	 * @brief �f�o�b�O�ɐݒ�
	*/
	void OnDebugMode()
	{
		m_debugMode = true;
	}
	/**
	 * @brief �f�o�b�O�łȂ�����
	*/
	void OffDebugMode()
	{
		m_debugMode = false;
	}
	/**
	 * @brief �f�o�b�O�̃\�����[�h�ɐݒ�
	*/
	void OnDebugSoloMode()
	{
		if (false == m_debugMode) {
			return;
		}

		m_debugSoloMode = true;
	}
	/**
	 * @brief �f�o�b�O�̃\�����[�h�łȂ�����
	*/
	void OffDebugSoloMode()
	{
		if (false == m_debugMode) {
			return;
		}

		m_debugSoloMode = false;
	}



private: // data menber
	////////////////////////////////////////////////////////////
	// �N���X�̃C���X�^���X
	////////////////////////////////////////////////////////////

	Player* m_player[2] = { nullptr };
	PlayerCamera* m_playerCamera = nullptr;

	////////////////////////////////////////////////////////////
	// �t���O
	////////////////////////////////////////////////////////////

	bool m_debugMode = false; // �f�o�b�O���[�h��
	bool m_debugSoloMode = false; // �f�o�b�O�̃\�����[�h��
};



/**
 * @brief �f�o�b�O�̂Ƃ��Ɏg�p���鏈��
*/
namespace nsDebug
{
	/*!
	 *@brief ���O�o��
	*/
	static inline void Log(const char* format, ...)
	{
		static char log[1024 * 10];
		va_list va;
		va_start(va, format);
		vsprintf(log, format, va);
		OutputDebugStringA(log);
		va_end(va);
	}

	static inline void LogW(const wchar_t* format, ...)
	{
		static wchar_t log[1024 * 10];
		va_list va;
		va_start(va, format);
		vswprintf_s(log, format, va);
		OutputDebugStringW(log);
		va_end(va);
	}


	/*!
	 * @brief �A�T�[�g
	 *@ param[in] flag �U�̂Ƃ��A�T�[�g���������܂��B
	*/
	static inline void MyAssert(bool flag, const char* format, const char* file, long line, ...)
	{
		if (!flag) {
			va_list va;
			va_start(va, flag);
			vprintf(format, va);
			char fileLineInfo[256];
			sprintf_s(fileLineInfo, "%s, %d�s��", file, line);
			char assertMessage[256];
			vsprintf_s(assertMessage, format, va);
			strcat_s(assertMessage, fileLineInfo);
			MessageBoxA(nullptr, assertMessage, "�A�T�[�g", MB_OK);
			va_end(va);
			std::abort();
		}
	}
}

#ifdef K2_DEBUG
#define 	K2_ASSERT( flg, format, ... )	nsK2EngineLow::MyAssert( flg, format, __FILE__, __LINE__, __VA_ARGS__)
#else
#define 	K2_ASSERT( flg, format, ... )
#endif
