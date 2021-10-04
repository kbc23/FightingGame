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
}