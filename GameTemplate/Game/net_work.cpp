#include "stdafx.h"
#include "net_work.h"

#include <random>

#include "my_debug.h"

// 358558fc-3cee-42c2-8bc1-65c7a4bfd315

NetWork* NetWork::m_instance = nullptr;



namespace
{
	const ExitGames::Common::JString PLAYER_NAME = L"user";
    const float MAX_FPS = 30.0f;	// �I�����C���ΐ펞�̍ő�FPS
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
	//���ׂẴv���C���[���Q�[���J�n�\�ɂȂ������ɌĂ΂��֐����Z�b�g
	m_allPlayerNotifyPossibleGameStartFunc = onAllPlayerPossibleGameStart;
	//���ׂẴv���C���[�����[���ɎQ���������ɌĂ΂��֐����Z�b�g
	m_allPlayerJoinedRoomFunc = onAllPlayerJoinedRoom;
	//�G���[�������ɌĂ΂��֐����Z�b�g
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

	// �Œ�t���[���̍ő�FPS30�ɐݒ肷��B
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
// Update()�ł̏���
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

	// �T�[�o�[�ɐڑ��ł����̂ŁA���������B
	ExitGames::LoadBalancing::RoomOptions roomOption;
	// �����̍ő�l���͓�l
	roomOption.setMaxPlayers(2);
	// sendDirect()�֐�(P2P)�̓���͒N�Ƃł����R�ɒʐM�ł��郂�[�h�B
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
		// 10�b+���b�o�߂����̂ŁA��U�T�[�o�[����ؒf���āA�Đڑ��B
		m_state = State::enDisconnecting;
		m_loadBalancingClient->disconnect();

		nsDebug::Log("Update_Joined()_End(disconnect)\n");
	}
	if (m_otherPlayerState == enOtherPlayerNetWorkState_joinedRoom) {
		// ���ׂẴv���C���[�����[���ɂ�������B
		// �v���C���[�����������邽�߂̏��𑗂�B
		SendInitDataOtherPlayer();
		m_timer = 0.0f;
		std::random_device rnd;
		m_waitLimitTime = 10.0f + rnd() % 30;
		// ���v���C���[�̏��������󂯎��҂��֑J�ڂ���B
		m_state = State::enWaitRecvInitDataOtherPlayer;

		nsDebug::Log("Update_Joined()_End\n");
	}
}

void NetWork::Update_WaitStartGame()
{
	nsDebug::Log("Update_WaitStartGame()_Start\n");

	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer > 1.0f) {
		// ������ɂ��̃f�[�^���͂��Ă��炸�A�����Ƃ����ő҂��Ƃ��������̂ŁA
		// 1�b���ƂɃv���C���[�����������邽�߂̃f�[�^���đ�����B
		SendInitDataOtherPlayer();
		m_timer = 0.0f;

		nsDebug::Log("Update_WaitStartGame()_End(SendInitDataOtherPlayer)\n");
	}

	if (m_timer > m_waitLimitTime) {
		// 10�b+���b�҂��Ă��p�P�b�g���͂��Ȃ������̂ŁA��U�ؒf���āA�Đڑ��B
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

	// �p�b�h�f�[�^�𑗐M�B
	if (m_frameNo < 3) {
		// 3�t���[������������B
		SendPadDataDirect();
		m_frameNo++;
	}
	else {
		// �o�b�t�@�����O�I���B
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
			// ���v���C���[���������甲�����B
			m_errorFunc();
			m_loadBalancingClient->disconnect();
			break;
		}
		auto it = m_padData[otherPlNo].find(m_playFrameNo);
		if (it != m_padData[otherPlNo].end()) {
#ifdef ENABLE_ONLINE_PAD_LOG
			// �Đ������p�b�h�̃��O���o�́B
			OutputPlayPadDataLog();
#endif
			// �Đ��t���[���̃p�b�h�����󂯎���Ă���B
			m_pad[plNo].Update(m_padData[plNo][m_playFrameNo].xInputState);
			m_pad[otherPlNo].Update(it->second.xInputState);
			// �Đ��ς݂̃p�b�h�����폜�B
			m_padData[plNo].erase(m_playFrameNo);
			m_padData[otherPlNo].erase(m_playFrameNo);
			break;
		}
		else {
			// �f�[�^�����Ă��Ȃ� or �p�P�b�g���X�g
			// �đ����N�G�X�g�𑗂�B
			SendRequestResendPadDataDirect(m_playFrameNo);
			loopCount++;
			Sleep(100);
			m_loadBalancingClient->service();
			if (loopCount == 100) {
				// �ڑ��G���[�B
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
// ���̑�
////////////////////////////////////////////////////////////

void NetWork::SendPossibleGameStart()
{
	nsDebug::Log("SendPossibleGameStart()_Start\n");

	// �Q�[���J�n�x���g�𑗐M�B
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

	// ���[���ɃW���C���������Ƃ�ʒm�B
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

	// ����p�b�h�f�[�^���\�z����B
	SPadData padData;
	padData.dataType = enDirectMessageType_PadData;
	padData.xInputState = g_pad[0]->GetXInputState();
	padData.frameNo = m_frameNo;
	// �`�F�b�N�T���p�̃f�[�^��ǉ�����B
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
// �I�[�o�[���C�h
////////////////////////////////////////////////////////////

void NetWork::disconnectReturn(void)
{
	// �ؒf�ς݂ɂ���B
	m_state = State::enDisconnected;
}

void NetWork::connectionErrorReturn(int errorCode)
{
	// �ڑ��Ɏ��s�����̂ŁA�ؒf�ς݂ɂ���B
	m_state = State::enDisconnected;
}

void NetWork::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
	if (m_playerType == enPlayerNetWorkType_host
		&& playerNr == 2
		) {
		// �N���C�A���g���W���C�����Ă����̂ŁA���v���C���[���W���C���ς݂ɂ���B
		m_otherPlayerState = enOtherPlayerNetWorkState_joinedRoom;
	}
}

////////////////////////////////////////////////////////////
// ���̑�
////////////////////////////////////////////////////////////

void NetWork::OnDirectMessageType_PadData(std::uint8_t* pData, int size)
{
	nsDebug::Log("OnDirectMessageType_PadData()_Start\n");

	// �p�b�h���
	SPadData padData;
	memcpy(&padData, pData, size);
	// �`�F�b�N�T���𗘗p������茟�o���s���B
	// �����Ă����f�[�^�̃`�F�b�N�T���p�̃f�[�^���v�Z�B
	unsigned int checksum = CalcCheckSum(&padData, sizeof(padData) - 4);

	int otherPlNo = GetOtherPlayerNo();
	// �v�Z�����l�Ƒ����Ă����l�����������ׂ�B
	if (checksum == padData.checksum) {
		// �`�F�b�N�T���ʉ߁B
		// ���͋N���Ă��Ȃ��\���������B
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

	// �p�b�h�f�[�^�̍đ����N�G�X�g���󂯂��̂ŁA�ߋ��̃p�b�h�f�[�^���đ�����B
	SRequestResendPadData reqResendPadData;
	memcpy(&reqResendPadData, pData, size);

	int plNo = GetPlayerNo();
	auto it = m_padData[plNo].find(reqResendPadData.frameNo);
	if (it != m_padData[plNo].end()) {
		// �p�b�h�f�[�^���ł��Ă���B
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

	// ��������v���C���[���������̂ŁA�Q�[���I���B
	m_otherPlayerState = enOtherPlayerNetWorkState_leftRoom;

	nsDebug::Log("leaveRoomEventAction()_End\n");
}

void NetWork::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster)
{
	nsDebug::Log("connectReturn()_Start\n");

	if (errorCode)
	{
		// �T�[�o�[�ւ̐ڑ��G���[�����������̂ŁA�ؒf�ς݂ɂ���B
		nsDebug::LogW(errorString.toString());
		m_state = State::enDisconnected;
		return;
	}
	// �����ɓ��ꂽ�B
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
			K2_ASSERT(!m_isHoge, "���Ă΂�Ă���");
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

	// �����Ă����f�[�^���R�s�[�B
	auto valueObj = (ExitGames::Common::ValueObject<std::uint8_t*>*) & msg;
	const int* sizes = valueObj->getSizes();
	std::uint8_t* pData = (std::uint8_t*)valueObj->getDataCopy();
	// �f�[�^�̐擪�Ƀf�[�^�^�C�v�������Ă���B
	int dataType = (int)(*pData);

	// �f�[�^�^�C�v�ɉ����ď����𕪊�B
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
		// ���������Ȃ������B
		// �X�e�[�^�X��ڑ��ς݂ɖ߂��āA�ēx���������B
		m_state = State::enConnected;

		nsDebug::Log("joinRandomOrCreateRoomReturn()_End(Error)\n");
		return;
	}
	if (localPlayerNr == 1) {
		// ������������z�X�g�B
		m_playerType = enPlayerNetWorkType_host;
	}
	else {
		// �N���C�A���g�Ȃ̂ŁA���łɃz�X�g�͂���͂��Ȃ̂ŁA
		// ���v���C���[�͕����ɃW���C���ς݂ɂ���B
		m_otherPlayerState = enOtherPlayerNetWorkState_joinedRoom;
		m_playerType = enPlayerNetWorkType_client;
	}
	// ���[���ɓ������B
	m_state = State::enJoined;

	nsDebug::Log("joinRandomOrCreateRoomReturn()_End\n");
}