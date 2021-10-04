#pragma once

/**
 * @brief �f�o�b�O�̂Ƃ��Ɏg�p���鏈��
*/
namespace myDebug
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
}