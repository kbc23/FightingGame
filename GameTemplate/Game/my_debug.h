#pragma once
#include "model_render.h"
#include "font_render.h"

class Player;
class PlayerCamera;
class GameData;

class MyDebug : public IGameObject
{
public:
	void Init();
	void Finish();
	bool Start() override;
	void Update() override;


private:
	////////////////////////////////////////////////////////////
	// �Q�[���I���֘A
	////////////////////////////////////////////////////////////

	void GameEndUpdate();

	bool CheckGameEnd();


private: // enum
	/**
	 * @brief ���s
	*/
	enum EnWinOrLose
	{
		win, // ����
		lose, // ����
		inTheGame // ������
	};

	EnWinOrLose m_winOrLose[2] = { inTheGame,inTheGame };


	enum class EnStatus
	{
		game, // �Q�[����
		finishGame // �Q�[���I��
	};

	EnStatus m_status = EnStatus::game;



private: // data member
	////////////////////////////////////////////////////////////
	// �N���X�̃C���X�^���X
	////////////////////////////////////////////////////////////

	Player* m_player[2] = { nullptr };
	PlayerCamera* m_playerCamera = nullptr;
	ModelRender* m_modelStage = nullptr; // �X�e�[�W�̃��f��
	GameData* m_gameData = nullptr;
	FontRender* m_fontWinOrLose = nullptr;

	////////////////////////////////////////////////////////////
	// �t���O
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////

	int m_debugCountGameEnd = 0;
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
