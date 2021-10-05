#pragma once
#include "Common-cpp/inc/Common.h"
#include "LoadBalancing-cpp/inc/Client.h"

/// �ȉ��ANetWork�N���X�̏����̃t���[�ƂȂ�܂��B
/// 1. Init()�֐����g�p���āA���������s���B
/// 2. ���t���[��Update()�֐����Ăяo���B
///		�ȉ��AUpdate�̏������L�q
///		2.1 Photon�N���E�h�T�[�o�[�ւ̐ڑ�
///		2.2 ���[�������݂��Ă�����A���[���ɃW���C���A�Ȃ���ΐV�K�쐬
///		2.3 ���[���Ƀ��[�U�[���Q�l��������A�L�����̏������ɕK�v�ȏ��𑼃v���C���[�ɑ��M
///		2.4 ��������񂪓͂�����A�Q�[�����ɒʒm�BInit()�֐��Ŏw�肳�ꂽonAllPlayerJoinedRoom()�֐����Ă΂�܂��B
///			2.4.1 �Q�[�����̓Q�[�����n�߂邽�߂̏������������s���A�������������NotifyPossibleStartPlayGame()�֐���
///				�@�Ăяo���āA�Q�[���J�n�\�ɂȂ������Ƃ��G���W���ɒʒm
///		2.5 �S�Ẵv���C���[���Q�[���\�ɂȂ�܂Ń|�[�����O
///			2.5.1 �S�Ẵv���C���[���Q�[���J�n�\�ɂȂ�ƁAInit()�֐��Ŏw�肳�ꂽonAllPlayerPossibleGameStart()�֐����Ăяo���܂��B
///		2.5 �p�b�h����3�t���[�����o�b�t�@�����O
///		2.6 �o�b�t�@�����O������A�Q�[���p�b�h�̍X�V�𖈃t���[���s���B

class NetWork : ExitGames::LoadBalancing::Listener
{
public:
	~NetWork();
	void Init(
		void* pSendData,
		int sendDataSize,
		std::function<void(void* pRecvData, int dataSize)> onAllPlayerJoinedRoom,
		std::function<void()> onAllPlayerPossibleGameStart,
		std::function<void()> onError
	);

	void Update();


private:
	////////////////////////////////////////////////////////////
	// Update()�ł̏���
	////////////////////////////////////////////////////////////
	/**
	 * @brief 
	 * �������X�e�b�v�̍X�V����
	 * Photon�T�[�o�[�ւ̐ڑ����N�G�X�g�𑗐M���܂�
	*/
	void Update_Initialized();
	/**
	 * @brief 
	 * Photon�T�[�o�[�ɐڑ��ς݂̎��̍X�V����
	 * Photon�T�[�o�[�ɕ������쐬���郊�N�G�X�g�𑗐M���܂�
	*/
	void Update_Connected();
	/**
	 * @brief 
	 * �����ɓ����Ă���Ƃ��̏���
	 * �����ɂ��ׂẴv���C���[�������ƁA�L�����N�^�[�̏��������𑗐M���܂�
	*/
	void Update_Joined();
	/**
	 * @brief 
	 * �Q�[���̊J�n�҂����̍X�V����
	 * �L�����N�^�[�̃��[�h���I���̂�҂��܂��B
	 * �Q�[��������NotifyPossibleStartPlayGame()�֐����Ă΂��܂ŁA
	 * Update�̏����͂��̃X�e�b�v�Ŏ~�܂�܂��B
	 * �Q�[�����̓L�����N�^�[�̃��[�h�Ȃǂ��������āA
	 * �Q�[�����J�n�ł���悤�ɂȂ�����ANotifyPossibleStartPlayGame()�֐����Ăяo���Ă��������B
	*/
	void Update_WaitStartGame();
	/**
	 * @brief �C���Q�[��(�p�b�h�f�[�^�̃o�b�t�@�����O��)
	*/
	void Update_InGameBufferingPadData();
	/**
	 * @brief 
	 * �C���Q�[�����̍X�V����
	 * �p�b�h�f�[�^����Y������t���[���̃p�b�h�����擾���āA
	 * �Q�[���p�b�h�̏����X�V���܂��B
	 * �ΐ�v���C���[�̊Y���t���[���̃p�b�h��񂪁A�܂��͂��Ă��Ȃ��ꍇ�́A
	 * �đ����N�G�X�g�𑗂�܂��B
	 * �đ����N�G�X�g�𑗂��Ă��p�b�h��񂪗��Ȃ��ꍇ�́A�ʐM�G���[���������Ă���Ɣ��f���āA
	 * �G���[���������s���܂��B
	*/
	void Update_InGame();


private:
	/// <summary>
	/// P2P�ʐM��enDirectMessageType_PadData�̃��b�Z�[�W�������Ă����Ƃ��̏����B
	/// </summary>
	void OnDirectMessageType_PadData(std::uint8_t* pData, int size);
	/// <summary>
	/// P2P�ʐM��enDirectMessageType_RequestResendPadData�̃��b�Z�[�W�������Ă����Ƃ��̏����B
	/// </summary>
	/// <param name="pData"></param>
	/// <param name="size"></param>
	void OnDirectMessageType_RequestResendPadData(std::uint8_t* pData, int size);
	/// <summary>
	/// ���[�U�[���������甲�����Ƃ��ɌĂ΂�鏈���B
	/// </summary>
	/// <param name="playerNr"></param>
	/// <param name="isInactive"></param>
	void leaveRoomEventAction(int playerNr, bool isInactive) override;
	/// <summary>
	/// photon�T�[�o�[�ւ̐ڑ����N�G�X�g�����s�����ꍇ�ɌĂяo�����R�[���o�b�N�֐��B
	/// </summary>
	/// <param name="errorCode">�G���[�R�[�h</param>
	/// <param name="errorString">�G���[������</param>
	/// <param name="region">���[�W����</param>
	/// <param name="cluster">�N���X�^�[</param>
	void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) override;
	/// <summary>
	/// opRaiseEvent()�֐��ő���ꂽ���b�Z�[�W����M�����ꍇ�ɁA�Ăяo�����R�[���o�b�N�֐��B
	/// onRaiseEvent()�֐��̓Q�[���T�[�o�[�o�R�Ń��b�Z�[�W�������܂��B
	/// ���b�Z�[�W�̒ʐM�v���g�R����TCP�ł��B
	/// </summary>
	/// <param name="playerNr"></param>
	/// <param name="eventCode"></param>
	/// <param name="eventContentObj"></param>
	void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) override;
	/// <summary>
	/// sendDirect()�֐�(P2P�ʐM)�ő���ꂽ���b�Z�[�W����M�����ꍇ�ɁA�Ăяo�����R�[���o�b�N�֐��B
	/// ���b�Z�[�W�̒ʐM�v���g�R����UDP�ł��B
	/// �C���Q�[�����̍������X�|���X���K�v�ȒʐM��sendDirect()�֐��𗘗p���āA���̃��b�Z�[�W�̎�M�̏�����
	/// ���̊֐��̒��ɋL�q���Ă��������B
	/// </summary>
	/// <param name="msg"></param>
	/// <param name="remoteID"></param>
	/// <param name="relay"></param>
	void onDirectMessage(const ExitGames::Common::Object& msg, int remoteID, bool relay) override;
	/// <summary>
	/// ���[�����쐬 or ���������Ƃ��ɌĂ΂�鏈���B
	/// </summary>
	/// <param name="localPlayerNr"></param>
	/// <param name="gameProperties"></param>
	/// <param name="playerProperties"></param>
	/// <param name="errorCode"></param>
	/// <param name="errorString"></param>
	void joinRandomOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) override;









	/// <summary>
	/// �Q�[���J�n�\�ɂȂ������Ƃ𑼃v���C���[�ɒʒm�B
	/// </summary>
	void SendPossibleGameStart();
	/// <summary>
	/// ���v���C���[�����������邽�߂̏��𑗂�B
	/// </summary>
	void SendInitDataOtherPlayer();
	/// <summary>
	/// �p�b�h����P2P�Œ��ڑ��M�B
	/// </summary>
	void SendPadDataDirect();
	/// <summary>
	/// �p�b�h�f�[�^�̍đ����N�G�X�g��P2P�ő��M
	/// </summary>
	/// <param name="frameNo">�đ����N�G�X�g���s���t���[���ԍ�</param>
	void SendRequestResendPadDataDirect(int frameNo);

	/// <summary>
	/// �`�F�b�N�T�����v�Z
	/// </summary>
	/// <param name="pData">�`�F�b�N�T�����v�Z����f�[�^�̐擪�A�h���X</param>
	/// <param name="size">�f�[�^�̃T�C�Y</param>
	unsigned int CalcCheckSum(void* pData, int size);



private: //override
	void connectionErrorReturn(int errorCode) override;
	void clientErrorReturn(int errorCode) override {}
	void warningReturn(int warningCode) override {}
	void serverErrorReturn(int errorCode) override {}
	void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override {}
	void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override;
	void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override {}
	void disconnectReturn(void) override;




public: //Get�֐�
	/// <summary>
	/// �v���C���[�ԍ����擾�B
	/// �z�X�g�Ȃ�0�A�N���C�A���g�Ȃ�1��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	int GetPlayerNo() const
	{
		if (m_playerType == enPlayerNetWorkType_host) {
			return 0;
		}
		// �N���C�A���g�B
		return 1;
	}

	/// <summary>
	/// �ΐ푊��̃v���C���[�ԍ����擾�B
	/// �z�X�g�Ȃ�1�A�N���C�A���g�Ȃ�1��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	int GetOtherPlayerNo() const
	{
		// �ΐ푊��̃v���C���[�ԍ��́A�����̔ԍ��̔��΁B
		return !GetPlayerNo();
	}

	/// <summary>
	/// �Q�[���p�b�h�̎擾�B
	/// </summary>
	/// <param name="no"></param>
	/// <returns></returns>
	GamePad& GetGamePad(int no)
	{
		return m_pad[no];
	}

	/// <summary>
	/// �Q�[���J�n�\�ɂȂ������Ƃ�ʒm�B
	/// </summary>
	void NotifyPossibleStartPlayGame()
	{
		m_isPossibleGameStart = true;
		SendPossibleGameStart();
	}




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


private: //enum
	/**
	 * @brief ���g�̃l�b�g���[�N�̏��
	*/
	enum class State
	{
		enInitialized,						// ������
		enConnecting,						// �T�[�o�[�ڑ���
		enConnected,						// �T�[�o�[�ɐڑ��ς�
		enJoining,							// ���[���ɃW���C����
		enJoined,							// ���[���ɃW���C���ς݁B
		enWaitRecvInitDataOtherPlayer,		// �J�n�f�[�^�̎󂯎��҂��B
		enWaitStartGame,					// �Q�[���̊J�n�҂�
		enInGameBufferingPadData,			// �p�b�h�f�[�^�̃o�b�t�@�����O��
		enInGame,							// �Q�[���v���C���B
		enDisconnecting,					// �T�[�o�[����ؒf���B
		enDisconnected						// �T�[�o�[����ؒf�ς݁B
	};

	State m_state = State::enInitialized;

	/**
	 * @brief �C�x���g
	*/
	enum EnEvent {
		enEvent_SendInitDataForOtherPlayer,	  // ���v���C���[�̏��������𑗂�
		enEvent_PossibleGameStartOtherPlayer, // ���v���C���[���Q�[���J�n�\�ɂȂ������Ƃ�ʒm
	};

	/**
	 * @brief sendDirect()�֐��ő����Ă��郁�b�Z�[�W�̎��
	*/
	enum EnDirectMessageType {
		enDirectMessageType_PadData,				// �p�b�h�f�[�^
		enDirectMessageType_RequestResendPadData,	// �p�b�h�f�[�^�̍đ����N�G�X�g
	};
	
	/**
	 * @brief �p�b�h�f�[�^
	*/
	struct SPadData {
		int dataType;				// �f�[�^�̎�ށB
		XINPUT_STATE xInputState;	// XInput�X�e�[�g
		int frameNo;				// �t���[���ԍ�
		unsigned int checksum;		// �`�F�b�N�T���p�̃f�[�^�B
	};

	/**
	 * @brief �p�b�h�f�[�^�̍đ����N�G�X�g
	*/
	struct SRequestResendPadData {
		int dataType;
		int frameNo;
	};

	/**
	 * @brief �v���C���[�̃l�b�g���[�N�^�C�v
	*/
	enum EnPlayerType {
		enPlayerNetWorkType_host, // �z�X�g(�����������)
		enPlayerNetWorkType_client, // �N���C�A���g(�����̕����ɓ�����)
		enPlayerNetWorkType_undef // �s��
	};

	EnPlayerType m_playerType = enPlayerNetWorkType_undef; // �v���C���[�̃^�C�v
	
	/**
	 * @brief ���v���C���[�̃l�b�g���[�N�̏��
	*/
	enum EnOtherPlayerState {
		enOtherPlayerNetWorkState_undef, // �s��
		enOtherPlayerNetWorkState_joinedRoom, // �����ɓ����Ă���
		enOtherPlayerNetWorkState_possibleGameStart, // �Q�[���J�n�\���
		enOtherPlayerNetWorkState_leftRoom // �������甲����
	};

	EnOtherPlayerState m_otherPlayerState = enOtherPlayerNetWorkState_undef; // ���v���C���[�̏�ԁB


private: //data menber
    static NetWork* m_instance;		//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B

	////////////////////////////////////////////////////////////
	// �֐��̃f�[�^��ۑ�����ϐ�
	////////////////////////////////////////////////////////////

	// ���ׂẴv���C���[�����[���ɎQ���������ɌĂ΂��֐���ۑ�����ϐ��̌^�̕ʖ�
	using OnAllPlayerJoinedRoom = std::function<void(void* pRecvData, int dataSize)>;
	// ���ׂẴv���C���[���Q�[�����J�n�ł����ԂɂȂ������ɌĂ΂��֐���ۑ�����ϐ��̌^�̕ʖ�
	using OnAllPlayerNotifyPossibleGameStart = std::function<void()>;
	// �G���[�������������ɌĂ΂��֐���ۑ�����ϐ��̌^�̕ʖ�
	using OnErrorFunc = std::function<void()>;

	// ���ׂẴv���C���[�����[���ɎQ���������ɌĂ΂��֐���ۑ�����ϐ�
	OnAllPlayerJoinedRoom m_allPlayerJoinedRoomFunc = nullptr;
	// ���ׂẴv���C���[���Q�[���J�n�\�ł��邱�Ƃ�ʒm�������ɌĂ΂��֐���ۑ�����ϐ�
	OnAllPlayerNotifyPossibleGameStart m_allPlayerNotifyPossibleGameStartFunc = nullptr;
	// �ʐM�G���[���N�������ɌĂ΂��֐�
	OnErrorFunc m_errorFunc = nullptr;

	////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////

	using LoadBalancingClientPtr = std::unique_ptr<ExitGames::LoadBalancing::Client>;

	LoadBalancingClientPtr m_loadBalancingClient;




	
	////////////////////////////////////////////////////////////
	// �f�[�^��ۑ�����|�C���^�ƃf�[�^�̃T�C�Y
	////////////////////////////////////////////////////////////

	std::unique_ptr<std::uint8_t[]> m_sendDataOnGameStart; // �Q�[���J�n���ɓ]������f�[�^�B
	int m_sendDataSizeOnGameStart; // �Q�[���J�n���ɓ]������f�[�^�̃T�C�Y�B



	////////////////////////////////////////////////////////////
	// �^�C�}�[
	////////////////////////////////////////////////////////////

	float m_timer = 0.0f; // �^�C�}�[
	float m_waitLimitTime = 0.0f; // �҂��̍ő�b���B


	////////////////////////////////////////////////////////////
	// �t���O
	////////////////////////////////////////////////////////////

	bool m_finishInit = false; // ������������


	bool m_isPossibleGameStart = false; // �Q�[���J�n�\�t���O


	bool m_isHoge = false;



	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////

	int m_frameNo = 0; // ���݂̃p�b�h�̃t���[���ԍ�
	std::map< int, SPadData> m_padData[2]; // �p�b�h�f�[�^	
	int m_playFrameNo = 0; // ���݂̃Q�[���i�s�t���[���ԍ�
	GamePad m_pad[2]; // �Q�[���p�b�h

	int m_recieveDataSize = 0; // �Q�[���J�n�̂��߂Ɏ󂯎�����f�[�^�̃T�C�Y

	std::unique_ptr<std::uint8_t[]> m_recieveDataOnGameStart; // �Q�[���J�n�̂��߂Ɏ󂯎�����f�[�^
};