#include "stdafx.h"
#include "net_work.h"

#include <random>

#include "my_debug.h"



// 358558fc-3cee-42c2-8bc1-65c7a4bfd315

NetWork* NetWork::m_instance = nullptr;

namespace
{
	const ExitGames::Common::JString PLAYER_NAME = L"user";
    const float MAX_FPS = 30.0f;	// オンライン対戦時の最大FPS
}

void NetWork::Init(
	void* pSendData,
	int sendDataSize,
	std::function<void(void* pRecvData, int dataSize)> onAllPlayerJoinedRoom,
	std::function<void()> onAllPlayerPossibleGameStart,
	std::function<void()> onError
)
{
	//すべてのプレイヤーがゲーム開始可能になった時に呼ばれる関数をセット
	m_allPlayerNotifyPossibleGameStartFunc = onAllPlayerPossibleGameStart;
	//すべてのプレイヤーがルームに参加した時に呼ばれる関数をセット
	m_allPlayerJoinedRoomFunc = onAllPlayerJoinedRoom;
	//エラー発生時に呼ばれる関数をセット
	m_errorFunc = onError;

	ExitGames::LoadBalancing::Client* p = new ExitGames::LoadBalancing::Client(
		*this,
		L"358558fc-3cee-42c2-8bc1-65c7a4bfd315",
		L"1.0",
		ExitGames::Photon::ConnectionProtocol::TCP,
		true
	);

	m_loadBalancingClient.reset(p);
	m_loadBalancingClient->setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // that instance of LoadBalancingClient and its implementation details
	ExitGames::Common::Base::setListener(this);
	ExitGames::Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // all classes that inherit from Base
	m_sendDataOnGameStart = std::make_unique<std::uint8_t[]>(sendDataSize);
	memcpy(m_sendDataOnGameStart.get(), pSendData, sendDataSize);
	m_sendDataSizeOnGameStart = sendDataSize;


	m_finishInit = true;
}

void NetWork::Update()
{
	if (false == m_finishInit) {
		return;
	}

	switch (m_state) {
	case State::enInitialized:
		Update_Initialized();
		break;
	case State::enConnected:
		Update_Connected();
		break;
	case State::enJoined:
		Update_Joined();
		break;
	case State::enWaitStartGame:
		Update_WaitStartGame();
		break;
	case State::enInGameBufferingPadData:
		Update_InGameBufferingPadData();
		break;
	case State::enInGame:
		Update_InGame();
		break;
	case State::enDisconnected:
		m_state = State::enInitialized;
		break;
	default:
		break;
	}

	m_loadBalancingClient->service();
}

////////////////////////////////////////////////////////////
// Update()での処理
////////////////////////////////////////////////////////////

void NetWork::Update_Initialized()
{
	myDebug::Log("Update_Initialized()\n");

	ExitGames::LoadBalancing::ConnectOptions connectOption;
	connectOption.setAuthenticationValues(ExitGames::LoadBalancing::AuthenticationValues().setUserID(ExitGames::Common::JString() + GETTIMEMS())).setUsername(PLAYER_NAME + GETTIMEMS());
	connectOption.setTryUseDatagramEncryption(true);
	m_loadBalancingClient->connect(connectOption);
	m_state = State::enConnecting;
}

void NetWork::Update_Connected()
{
	myDebug::Log("Update_Connected()\n");

	// サーバーに接続できたので、部屋を作る。
	ExitGames::LoadBalancing::RoomOptions roomOption;
	// 部屋の最大人数は二人
	roomOption.setMaxPlayers(2);
	// sendDirect()関数(P2P)の動作は誰とでも自由に通信できるモード。
	roomOption.setDirectMode(ExitGames::LoadBalancing::DirectMode::ALL_TO_ALL);

	m_loadBalancingClient->opJoinRandomOrCreateRoom(
		ExitGames::Common::JString(),
		roomOption,
		ExitGames::Common::Hashtable(),
		2
	);

	m_timer = 0.0f;
	std::random_device rnd;
	m_waitLimitTime = 10.0f + rnd() % 30;
	m_state = State::enJoining;
}

void NetWork::Update_Joined()
{
	myDebug::Log("Update_Joined()\n");

	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > m_waitLimitTime) {
		// 10秒+α秒経過したので、一旦サーバーから切断して、再接続。
		m_state = State::enDisconnecting;
		m_loadBalancingClient->disconnect();
	}
	if (m_otherPlayerState == enOtherPlayerNetWorkState_joinedRoom) {
		// すべてのプレイヤーがルームにそろった。
		// プレイヤーを初期化するための情報を送る。
		SendInitDataOtherPlayer();
		m_timer = 0.0f;
		std::random_device rnd;
		m_waitLimitTime = 10.0f + rnd() % 30;
		// 他プレイヤーの初期化情報受け取り待ちへ遷移する。
		m_state = State::enWaitRecvInitDataOtherPlayer;
	}
}

void NetWork::Update_WaitStartGame()
{
	myDebug::Log("Update_WaitStartGame()\n");

	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 1.0f) {
		// 相手方にこのデータが届いておらず、ずっとここで待つことがあったので、
		// 1秒ごとにプレイヤーを初期化するためのデータを再送する。
		SendInitDataOtherPlayer();
		m_timer = 0.0f;
	}

	if (m_timer > m_waitLimitTime) {
		// 10秒+α秒待ってもパケットが届かなかったので、一旦切断して、再接続。
		m_state = State::enDisconnecting;
		m_loadBalancingClient->disconnect();
	}
	if (m_otherPlayerState == enOtherPlayerNetWorkState_possibleGameStart
		&& m_isPossibleGameStart) {
		m_allPlayerNotifyPossibleGameStartFunc();
		m_state = State::enInGameBufferingPadData;
	}
}

void NetWork::Update_InGameBufferingPadData()
{
	myDebug::Log("Update_InGameBufferingPadData()\n");

	// パッドデータを送信。
	if (m_frameNo < 3) {
		// 3フレーム分だけ送る。
		SendPadDataDirect();
		m_frameNo++;
	}
	else {
		// バッファリング終わり。
		m_state = State::enInGame;
	}
}

void NetWork::Update_InGame()
{
	myDebug::Log("Update_InGame()\n");

	int loopCount = 0;
	int plNo = GetPlayerNo();
	int otherPlNo = GetOtherPlayerNo();

	while (true) {
		if (m_otherPlayerState == enOtherPlayerNetWorkState_leftRoom) {
			// 他プレイヤーが部屋から抜けた。
			m_errorFunc();
			m_loadBalancingClient->disconnect();
			break;
		}
		auto it = m_padData[otherPlNo].find(m_playFrameNo);
		if (it != m_padData[otherPlNo].end()) {
#ifdef ENABLE_ONLINE_PAD_LOG
			// 再生したパッドのログを出力。
			OutputPlayPadDataLog();
#endif
			// 再生フレームのパッド情報を受け取っている。
			m_pad[plNo].Update(m_padData[plNo][m_playFrameNo].xInputState);
			m_pad[otherPlNo].Update(it->second.xInputState);
			// 再生済みのパッド情報を削除。
			m_padData[plNo].erase(m_playFrameNo);
			m_padData[otherPlNo].erase(m_playFrameNo);
			break;
		}
		else {
			// データが来ていない or パケットロスト
			// 再送リクエストを送る。
			SendRequestResendPadDataDirect(m_playFrameNo);
			loopCount++;
			Sleep(100);
			m_loadBalancingClient->service();
			if (loopCount == 100) {
				// 接続エラー。
				m_errorFunc();
				m_loadBalancingClient->disconnect();
				break;
			}
		}
	}
	SendPadDataDirect();
	m_playFrameNo++;
	m_frameNo++;
}

////////////////////////////////////////////////////////////
// その他
////////////////////////////////////////////////////////////

void NetWork::SendInitDataOtherPlayer()
{
	// ルームにジョインしたことを通知。
	ExitGames::LoadBalancing::RaiseEventOptions eventOpt;
	ExitGames::Common::Hashtable event;

	event.put(0, m_sendDataOnGameStart.get(), m_sendDataSizeOnGameStart);

	m_loadBalancingClient->opRaiseEvent(
		true,
		event,
		enEvent_SendInitDataForOtherPlayer,
		eventOpt
	);
}

void NetWork::SendPadDataDirect()
{
	myDebug::Log("SendPadData:frameNo = %d\n", m_frameNo);

	// 送るパッドデータを構築する。
	SPadData padData;
	padData.dataType = enDirectMessageType_PadData;
	padData.xInputState = g_pad[0]->GetXInputState();
	padData.frameNo = m_frameNo;
	// チェックサム用のデータを追加する。
	padData.checksum = CalcCheckSum(&padData, sizeof(SPadData) - 4);

	int plNo = GetPlayerNo();
	auto itFind = m_padData[plNo].find(m_frameNo);
	m_padData[plNo].insert({ m_frameNo , padData });
	m_loadBalancingClient->sendDirect(
		(std::uint8_t*)&padData,
		sizeof(padData)
	);
}

void NetWork::SendRequestResendPadDataDirect(int frameNo)
{
	myDebug::Log("RequestResendPadData : frameNo = %d\n", frameNo);

	SRequestResendPadData reqResendPadData;
	reqResendPadData.dataType = enDirectMessageType_RequestResendPadData;
	reqResendPadData.frameNo = frameNo;

	m_loadBalancingClient->sendDirect(
		(std::uint8_t*)&reqResendPadData,
		sizeof(reqResendPadData)
	);
}

unsigned int NetWork::CalcCheckSum(void* pData, int size)
{
	std::uint8_t* p = reinterpret_cast<std::uint8_t*>(pData);
	unsigned int checkSum = 0;
	for (int i = 0; i < size; i++) {
		checkSum += p[i] + i;
	}

	return checkSum;
}

////////////////////////////////////////////////////////////
// オーバーライド
////////////////////////////////////////////////////////////

void NetWork::disconnectReturn(void)
{
	// 切断済みにする。
	m_state = State::enDisconnected;
}

void NetWork::connectionErrorReturn(int errorCode)
{
	// 接続に失敗したので、切断済みにする。
	m_state = State::enDisconnected;
}

void NetWork::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
	if (m_playerType == enPlayerNetWorkType_host
		&& playerNr == 2
		) {
		// クライアントがジョインしてきたので、他プレイヤーをジョイン済みにする。
		m_otherPlayerState = enOtherPlayerNetWorkState_joinedRoom;
	}
}