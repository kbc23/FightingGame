#include "stdafx.h"
#include "net_work.h"



// 358558fc-3cee-42c2-8bc1-65c7a4bfd315

NetWork* NetWork::m_instance = nullptr;

namespace
{
    const float MAX_FPS = 30.0f;	// ƒIƒ“ƒ‰ƒCƒ“‘Îí‚ÌÅ‘åFPS
}

void NetWork::Init()
{
	const wchar_t* test1 = L"358558fc-3cee-42c2-8bc1-65c7a4bfd315";
	const wchar_t* test2 = L"1.0";


	ExitGames::LoadBalancing::Client* p = new ExitGames::LoadBalancing::Client(
		*this,
		test1,
		test2,
		ExitGames::Photon::ConnectionProtocol::TCP,
		true
	);

}