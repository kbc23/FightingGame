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



NetWork::~NetWork()
{
	//g_engine->SetFrameRateMode(
	//	m_frameRateInfoBackup.frameRateMode,
	//	m_frameRateInfoBackup.maxFPS
	//);

	m_loadBalancingClient->opLeaveRoom();
	m_loadBalancingClient->opLeaveLobby();
	m_loadBalancingClient->disconnect();
#ifdef ENABLE_ONLINE_PAD_LOG
	fclose(m_fpLog);
#endif
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

	// 固定フレームの最大FPS30に設定する。
	m_frameRateInfoBackup = g_engine->GetFrameRateInfo();
	g_engine->SetFrameRateMode(TkEngine::EnFrameRateMode::enFrameRateMode_Fix, static_cast<int>(MAX_FPS));

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
	nsDebug::Log("Update_Initialized()_Start\n");

	ExitGames::LoadBalancing::ConnectOptions connectOption;
	connectOption.setAuthenticationValues(ExitGames::LoadBalancing::AuthenticationValues().setUserID(ExitGames::Common::JString() + GETTIMEMS())).setUsername(PLAYER_NAME + GETTIMEMS());
	connectOption.setTryUseDatagramEncryption(true);
	m_loadBalancingClient->connect(connectOption);
	m_state = State::enConnecting;

	nsDebug::Log("Update_Initialized()_End\n");
}

void NetWork::Update_Connected()
{
	nsDebug::Log("Update_Connected()_Start\n");

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

	nsDebug::Log("Update_Connected()_End\n");
}

void NetWork::Update_Joined()
{
	nsDebug::Log("Update_Joined()_Start\n");

	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > m_waitLimitTime) {
		// 10秒+α秒経過したので、一旦サーバーから切断して、再接続。
		m_state = State::enDisconnecting;
		m_loadBalancingClient->disconnect();

		nsDebug::Log("Update_Joined()_End(disconnect)\n");
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

		nsDebug::Log("Update_Joined()_End\n");
	}
}

void NetWork::Update_WaitStartGame()
{
	nsDebug::Log("Update_WaitStartGame()_Start\n");

	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 1.0f) {
		// 相手方にこのデータが届いておらず、ずっとここで待つことがあったので、
		// 1秒ごとにプレイヤーを初期化するためのデータを再送する。
		SendInitDataOtherPlayer();
		m_timer = 0.0f;

		nsDebug::Log("Update_WaitStartGame()_End(SendInitDataOtherPlayer)\n");
	}

	if (m_timer > m_waitLimitTime) {
		// 10秒+α秒待ってもパケットが届かなかったので、一旦切断して、再接続。
		m_state = State::enDisconnecting;
		m_loadBalancingClient->disconnect();

		nsDebug::Log("Update_WaitStartGame()_End(disconnect)\n");
	}
	if (m_otherPlayerState == enOtherPlayerNetWorkState_possibleGameStart
		&& m_isPossibleGameStart) {
		m_allPlayerNotifyPossibleGameStartFunc();
		m_state = State::enInGameBufferingPadData;

		nsDebug::Log("Update_WaitStartGame()_End(AllPlayerNotifyPossibleGameStart)\n");
	}
}

void NetWork::Update_InGameBufferingPadData()
{
	nsDebug::Log("Update_InGameBufferingPadData()\n");

	// パッドデータを送信。
	if (m_frameNo < 3) {
		// 3フレーム分だけ送る。
		SendPadDataDirect();
		m_frameNo++;
	}
	else {
		// バッファリング終わり。
		m_state = State::enInGame;

		nsDebug::Log("Update_InGameBufferingPadData()_End\n");
	}
}

void NetWork::Update_InGame()
{
	nsDebug::Log("Update_InGame()_Start\n");

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

void NetWork::SendPossibleGameStart()
{
	nsDebug::Log("SendPossibleGameStart()_Start\n");

	// ゲーム開始ベントを送信。
	ExitGames::LoadBalancing::RaiseEventOptions eventOpt;

	ExitGames::Common::Hashtable event;

	m_loadBalancingClient->opRaiseEvent(
		true,
		event,
		enEvent_PossibleGameStartOtherPlayer,
		eventOpt
	);

	nsDebug::Log("SendPossibleGameStart()_End\n");
}

void NetWork::SendInitDataOtherPlayer()
{
	nsDebug::Log("SendInitDataOtherPlayer()_Start\n");

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

	nsDebug::Log("SendInitDataOtherPlayer()_End\n");
}

void NetWork::SendPadDataDirect()
{
	nsDebug::Log("SendPadDataDirect()_Start\n");
	nsDebug::Log("SendPadData:frameNo = %d\n", m_frameNo);

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

	nsDebug::Log("SendPadDataDirect()_End\n");
}

void NetWork::SendRequestResendPadDataDirect(int frameNo)
{
	nsDebug::Log("SendRequestResendPadDataDirect()_Start\n");
	nsDebug::Log("RequestResendPadData : frameNo = %d\n", frameNo);

	SRequestResendPadData reqResendPadData;
	reqResendPadData.dataType = enDirectMessageType_RequestResendPadData;
	reqResendPadData.frameNo = frameNo;

	m_loadBalancingClient->sendDirect(
		(std::uint8_t*)&reqResendPadData,
		sizeof(reqResendPadData)
	);

	nsDebug::Log("SendRequestResendPadDataDirect()_End\n");
}

unsigned int NetWork::CalcCheckSum(void* pData, int size)
{
	nsDebug::Log("CalcCheckSum()_Start\n");

	std::uint8_t* p = reinterpret_cast<std::uint8_t*>(pData);
	unsigned int checkSum = 0;
	for (int i = 0; i < size; i++) {
		checkSum += p[i] + i;
	}

	nsDebug::Log("CalcCheckSum()_End\n");

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

////////////////////////////////////////////////////////////
// その他
////////////////////////////////////////////////////////////

void NetWork::OnDirectMessageType_PadData(std::uint8_t* pData, int size)
{
	nsDebug::Log("OnDirectMessageType_PadData()_Start\n");

	// パッド情報
	SPadData padData;
	memcpy(&padData, pData, size);
	// チェックサムを利用した誤り検出を行う。
	// 送られてきたデータのチェックサム用のデータを計算。
	unsigned int checksum = CalcCheckSum(&padData, sizeof(padData) - 4);

	int otherPlNo = GetOtherPlayerNo();
	// 計算した値と送られてきた値が同じか調べる。
	if (checksum == padData.checksum) {
		// チェックサム通過。
		// 誤りは起きていない可能性が高い。
		auto it = m_padData[otherPlNo].find(padData.frameNo);
		if (it == m_padData[otherPlNo].end()) {
			// 
			m_padData[otherPlNo].insert({ padData.frameNo , padData });
		}
	}

	nsDebug::Log("OnDirectMessageType_PadData()_End\n");
}

void NetWork::OnDirectMessageType_RequestResendPadData(std::uint8_t* pData, int size)
{
	nsDebug::Log("OnDirectMessageType_RequestResendPadData()_Start\n");

	// パッドデータの再送リクエストを受けたので、過去のパッドデータを再送する。
	SRequestResendPadData reqResendPadData;
	memcpy(&reqResendPadData, pData, size);

	int plNo = GetPlayerNo();
	auto it = m_padData[plNo].find(reqResendPadData.frameNo);
	if (it != m_padData[plNo].end()) {
		// パッドデータができている。
		m_loadBalancingClient->sendDirect(
			(std::uint8_t*)&m_padData[plNo][reqResendPadData.frameNo],
			sizeof(m_padData[plNo][reqResendPadData.frameNo])
		);
	}

	nsDebug::Log("OnDirectMessageType_RequestResendPadData()_End\n");
}

void NetWork::leaveRoomEventAction(int playerNr, bool isInactive)
{
	nsDebug::Log("leaveRoomEventAction()_Start\n");

	// 部屋からプレイヤーが抜けたので、ゲーム終了。
	m_otherPlayerState = enOtherPlayerNetWorkState_leftRoom;

	nsDebug::Log("leaveRoomEventAction()_End\n");
}

void NetWork::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster)
{
	nsDebug::Log("connectReturn()_Start\n");

	if (errorCode)
	{
		// サーバーへの接続エラーが発生したので、切断済みにする。
		nsDebug::LogW(errorString.toString());
		m_state = State::enDisconnected;
		return;
	}
	// 部屋に入れた。
	m_state = State::enConnected;

	nsDebug::Log("connectReturn()_End\n");
}

void NetWork::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)
{
	nsDebug::Log("customEventAction()_Start\n");

	auto eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();
	switch (eventCode) {
	case enEvent_SendInitDataForOtherPlayer:
		if (m_state == State::enWaitRecvInitDataOtherPlayer) {
			K2_ASSERT(!m_isHoge, "二回呼ばれている");
			m_isHoge = true;
			nsDebug::Log("enEvent_SendInitDataForOtherPlayer\n");
			auto valuObj = (ExitGames::Common::ValueObject<std::uint8_t*>*)(eventContent.getValue(0));
			m_recieveDataSize = valuObj->getSizes()[0];
			m_recieveDataOnGameStart = std::make_unique<std::uint8_t[]>(m_recieveDataSize);
			auto pSrcData = valuObj->getDataCopy();
			memcpy(m_recieveDataOnGameStart.get(), pSrcData, m_recieveDataSize);
			m_allPlayerJoinedRoomFunc(m_recieveDataOnGameStart.get(), m_recieveDataSize);
			m_state = State::enWaitStartGame;
		}

		nsDebug::Log("customEventAction()_End_1\n");
		break;
	case enEvent_PossibleGameStartOtherPlayer:
		m_otherPlayerState = enOtherPlayerNetWorkState_possibleGameStart;

		nsDebug::Log("customEventAction()_End_2\n");
		break;
	}
}

void NetWork::onDirectMessage(const ExitGames::Common::Object& msg, int remoteID, bool relay)
{
	nsDebug::Log("onDirectMessage()_Start\n");

	// 送られてきたデータをコピー。
	auto valueObj = (ExitGames::Common::ValueObject<std::uint8_t*>*) & msg;
	const int* sizes = valueObj->getSizes();
	std::uint8_t* pData = (std::uint8_t*)valueObj->getDataCopy();
	// データの先頭にデータタイプがつけられている。
	int dataType = (int)(*pData);

	// データタイプに応じて処理を分岐。
	switch (dataType) {
	case enDirectMessageType_PadData:
		OnDirectMessageType_PadData(pData, sizes[0]);
		break;
	case enDirectMessageType_RequestResendPadData:
		OnDirectMessageType_RequestResendPadData(pData, sizes[0]);
		break;
	}

	nsDebug::Log("onDirectMessage()_End\n");
}

void NetWork::joinRandomOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
{
	nsDebug::Log("joinRandomOrCreateRoomReturn()_Start\n");

	if (errorCode) {
		// 部屋を作れなかった。
		// ステータスを接続済みに戻して、再度部屋を作る。
		m_state = State::enConnected;

		nsDebug::Log("joinRandomOrCreateRoomReturn()_End(Error)\n");
		return;
	}
	if (localPlayerNr == 1) {
		// 部屋を作ったホスト。
		m_playerType = enPlayerNetWorkType_host;
	}
	else {
		// クライアントなので、すでにホストはいるはずなので、
		// 他プレイヤーは部屋にジョイン済みにする。
		m_otherPlayerState = enOtherPlayerNetWorkState_joinedRoom;
		m_playerType = enPlayerNetWorkType_client;
	}
	// ルームに入った。
	m_state = State::enJoined;

	nsDebug::Log("joinRandomOrCreateRoomReturn()_End\n");
}