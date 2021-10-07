#pragma once

/**
 * @brief デバッグのときに使用する処理
*/
namespace myDebug
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

	bool DEBUG_soloMode = false;
}

#ifdef K2_DEBUG
#define 	K2_ASSERT( flg, format, ... )	nsK2EngineLow::MyAssert( flg, format, __FILE__, __LINE__, __VA_ARGS__)
#else
#define 	K2_ASSERT( flg, format, ... )
#endif
