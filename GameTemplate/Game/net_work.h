#pragma once
#include "Common-cpp/inc/Common.h"
#include "LoadBalancing-cpp/inc/Client.h"

class NetWork : ExitGames::LoadBalancing::Listener
{
public:
	void Init();




public:
	/// <summary>
	/// �C���X�^���X�̍쐬�B
	/// </summary>
	static void CreateInstance()
	{
		//m_instance = new NetWork;
	}
	/// <summary>
	/// �C���X�^���X�̔j���B
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}
	/// <summary>
	/// �C���X�^���X���擾�B
	/// </summary>
	/// <returns></returns>
	static NetWork* GetInstance()
	{
		return m_instance;
	}



private: //data menber
    static NetWork* m_instance;		//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B





};