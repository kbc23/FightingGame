#pragma once
#include "Common-cpp/inc/Common.h"
#include "LoadBalancing-cpp/inc/Client.h"

class NetWork : ExitGames::LoadBalancing::Listener
{
public:
	void Init();




public:
	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		//m_instance = new NetWork;
	}
	/// <summary>
	/// インスタンスの破棄。
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}
	/// <summary>
	/// インスタンスを取得。
	/// </summary>
	/// <returns></returns>
	static NetWork* GetInstance()
	{
		return m_instance;
	}



private: //data menber
    static NetWork* m_instance;		//唯一のインスタンスのアドレスを記録する変数。





};