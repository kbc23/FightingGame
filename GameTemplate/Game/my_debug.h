#pragma once

class Actor;
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
	///// <summary>
	///// インスタンスの作成。
	///// </summary>
	//static void CreateInstance()
	//{
	//	m_instance = new MyDebug;
	//}
	///// <summary>
	///// インスタンスの破棄。
	///// </summary>
	//static void DeleteInstance()
	//{
	//	delete m_instance;
	//}
	///// <summary>
	///// インスタンスを取得。
	///// </summary>
	///// <returns></returns>
	//static MyDebug* GetInstance()
	//{
	//	return m_instance;
	//}


public:
	/**
	 * @brief デバッグモードか
	 * @return デバッグモードかどうか
	*/
	const bool GetDebugMode()
	{
		return m_debugMode;
	}
	/**
	 * @brief デバッグのソロモードか
	 * @return デバッグのソロモードかどうか
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
	 * @brief デバッグに設定
	*/
	void OnDebugMode()
	{
		m_debugMode = true;
	}
	/**
	 * @brief デバッグでなくする
	*/
	void OffDebugMode()
	{
		m_debugMode = false;
	}
	/**
	 * @brief デバッグのソロモードに設定
	*/
	void OnDebugSoloMode()
	{
		if (false == m_debugMode) {
			return;
		}

		m_debugSoloMode = true;
	}
	/**
	 * @brief デバッグのソロモードでなくする
	*/
	void OffDebugSoloMode()
	{
		if (false == m_debugMode) {
			return;
		}

		m_debugSoloMode = false;
	}



private: // data menber
	//static MyDebug* m_instance;		//唯一のインスタンスのアドレスを記録する変数

	////////////////////////////////////////////////////////////
	// クラスのインスタンス
	////////////////////////////////////////////////////////////

	Actor* m_actor[2] = { nullptr };
	Player* m_player[2] = { nullptr };
	PlayerCamera* m_playerCamera = nullptr;

	////////////////////////////////////////////////////////////
	// フラグ
	////////////////////////////////////////////////////////////

	bool m_debugMode = false; // デバッグモードか
	bool m_debugSoloMode = false; // デバッグのソロモードか
};



/**
 * @brief デバッグのときに使用する処理
*/
namespace nsDebug
{
	/*!
	 *@brief ログ出力
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
	 * @brief アサート
	 *@ param[in] flag 偽のときアサートが発生します。
	*/
	static inline void MyAssert(bool flag, const char* format, const char* file, long line, ...)
	{
		if (!flag) {
			va_list va;
			va_start(va, flag);
			vprintf(format, va);
			char fileLineInfo[256];
			sprintf_s(fileLineInfo, "%s, %d行目", file, line);
			char assertMessage[256];
			vsprintf_s(assertMessage, format, va);
			strcat_s(assertMessage, fileLineInfo);
			MessageBoxA(nullptr, assertMessage, "アサート", MB_OK);
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
