#pragma once
#include "Common-cpp/inc/Common.h"
#include "LoadBalancing-cpp/inc/Client.h"

/// 以下、NetWorkクラスの処理のフローとなります。
/// 1. Init()関数を使用して、初期化を行う。
/// 2. 毎フレームUpdate()関数を呼び出す。
///		以下、Updateの処理を記述
///		2.1 Photonクラウドサーバーへの接続
///		2.2 ルームが存在していたら、ルームにジョイン、なければ新規作成
///		2.3 ルームにユーザーが２人揃ったら、キャラの初期化に必要な情報を他プレイヤーに送信
///		2.4 初期化情報が届いたら、ゲーム側に通知。Init()関数で指定されたonAllPlayerJoinedRoom()関数が呼ばれます。
///			2.4.1 ゲーム側はゲームを始めるための初期化処理を行い、初期化完了後にNotifyPossibleStartPlayGame()関数を
///				　呼び出して、ゲーム開始可能になったことをエンジンに通知
///		2.5 全てのプレイヤーがゲーム可能になるまでポーリング
///			2.5.1 全てのプレイヤーがゲーム開始可能になると、Init()関数で指定されたonAllPlayerPossibleGameStart()関数を呼び出します。
///		2.5 パッド情報を3フレーム分バッファリング
///		2.6 バッファリング完了後、ゲームパッドの更新を毎フレーム行う。

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
	// Update()での処理
	////////////////////////////////////////////////////////////
	/**
	 * @brief 
	 * 初期化ステップの更新処理
	 * Photonサーバーへの接続リクエストを送信します
	*/
	void Update_Initialized();
	/**
	 * @brief 
	 * Photonサーバーに接続済みの時の更新処理
	 * Photonサーバーに部屋を作成するリクエストを送信します
	*/
	void Update_Connected();
	/**
	 * @brief 
	 * 部屋に入っているときの処理
	 * 部屋にすべてのプレイヤーが揃うと、キャラクターの初期化情報を送信します
	*/
	void Update_Joined();
	/**
	 * @brief 
	 * ゲームの開始待ち中の更新処理
	 * キャラクターのロードが終わるのを待ちます。
	 * ゲーム側からNotifyPossibleStartPlayGame()関数が呼ばれるまで、
	 * Updateの処理はこのステップで止まります。
	 * ゲーム側はキャラクターのロードなどが完了して、
	 * ゲームを開始できるようになったら、NotifyPossibleStartPlayGame()関数を呼び出してください。
	*/
	void Update_WaitStartGame();
	/**
	 * @brief インゲーム(パッドデータのバッファリング中)
	*/
	void Update_InGameBufferingPadData();
	/**
	 * @brief 
	 * インゲーム中の更新処理
	 * パッドデータから該当するフレームのパッド情報を取得して、
	 * ゲームパッドの情報を更新します。
	 * 対戦プレイヤーの該当フレームのパッド情報が、まだ届いていない場合は、
	 * 再送リクエストを送ります。
	 * 再送リクエストを送ってもパッド情報が来ない場合は、通信エラーが発生していると判断して、
	 * エラー処理を実行します。
	*/
	void Update_InGame();


private:
	/// <summary>
	/// P2P通信でenDirectMessageType_PadDataのメッセージが送られてきたときの処理。
	/// </summary>
	void OnDirectMessageType_PadData(std::uint8_t* pData, int size);
	/// <summary>
	/// P2P通信でenDirectMessageType_RequestResendPadDataのメッセージが送られてきたときの処理。
	/// </summary>
	/// <param name="pData"></param>
	/// <param name="size"></param>
	void OnDirectMessageType_RequestResendPadData(std::uint8_t* pData, int size);
	/// <summary>
	/// ユーザーが部屋から抜けたときに呼ばれる処理。
	/// </summary>
	/// <param name="playerNr"></param>
	/// <param name="isInactive"></param>
	void leaveRoomEventAction(int playerNr, bool isInactive) override;
	/// <summary>
	/// photonサーバーへの接続リクエストを実行した場合に呼び出されるコールバック関数。
	/// </summary>
	/// <param name="errorCode">エラーコード</param>
	/// <param name="errorString">エラー文字列</param>
	/// <param name="region">リージョン</param>
	/// <param name="cluster">クラスター</param>
	void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) override;
	/// <summary>
	/// opRaiseEvent()関数で送られたメッセージを受信した場合に、呼び出されるコールバック関数。
	/// onRaiseEvent()関数はゲームサーバー経由でメッセージが送られます。
	/// メッセージの通信プロトコルはTCPです。
	/// </summary>
	/// <param name="playerNr"></param>
	/// <param name="eventCode"></param>
	/// <param name="eventContentObj"></param>
	void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) override;
	/// <summary>
	/// sendDirect()関数(P2P通信)で送られたメッセージを受信した場合に、呼び出されるコールバック関数。
	/// メッセージの通信プロトコルはUDPです。
	/// インゲーム中の高いレスポンスが必要な通信はsendDirect()関数を利用して、そのメッセージの受信の処理を
	/// この関数の中に記述してください。
	/// </summary>
	/// <param name="msg"></param>
	/// <param name="remoteID"></param>
	/// <param name="relay"></param>
	void onDirectMessage(const ExitGames::Common::Object& msg, int remoteID, bool relay) override;
	/// <summary>
	/// ルームを作成 or 入室したときに呼ばれる処理。
	/// </summary>
	/// <param name="localPlayerNr"></param>
	/// <param name="gameProperties"></param>
	/// <param name="playerProperties"></param>
	/// <param name="errorCode"></param>
	/// <param name="errorString"></param>
	void joinRandomOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) override;









	/// <summary>
	/// ゲーム開始可能になったことを他プレイヤーに通知。
	/// </summary>
	void SendPossibleGameStart();
	/// <summary>
	/// 他プレイヤーを初期化するための情報を送る。
	/// </summary>
	void SendInitDataOtherPlayer();
	/// <summary>
	/// パッド情報をP2Pで直接送信。
	/// </summary>
	void SendPadDataDirect();
	/// <summary>
	/// パッドデータの再送リクエストをP2Pで送信
	/// </summary>
	/// <param name="frameNo">再送リクエストを行うフレーム番号</param>
	void SendRequestResendPadDataDirect(int frameNo);

	/// <summary>
	/// チェックサムを計算
	/// </summary>
	/// <param name="pData">チェックサムを計算するデータの先頭アドレス</param>
	/// <param name="size">データのサイズ</param>
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




public: //Get関数
	/// <summary>
	/// プレイヤー番号を取得。
	/// ホストなら0、クライアントなら1を返します。
	/// </summary>
	/// <returns></returns>
	int GetPlayerNo() const
	{
		if (m_playerType == enPlayerNetWorkType_host) {
			return 0;
		}
		// クライアント。
		return 1;
	}

	/// <summary>
	/// 対戦相手のプレイヤー番号を取得。
	/// ホストなら1、クライアントなら1を返します。
	/// </summary>
	/// <returns></returns>
	int GetOtherPlayerNo() const
	{
		// 対戦相手のプレイヤー番号は、自分の番号の反対。
		return !GetPlayerNo();
	}

	/// <summary>
	/// ゲームパッドの取得。
	/// </summary>
	/// <param name="no"></param>
	/// <returns></returns>
	GamePad& GetGamePad(int no)
	{
		return m_pad[no];
	}

	/// <summary>
	/// ゲーム開始可能になったことを通知。
	/// </summary>
	void NotifyPossibleStartPlayGame()
	{
		m_isPossibleGameStart = true;
		SendPossibleGameStart();
	}




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


private: //enum
	/**
	 * @brief 自身のネットワークの状態
	*/
	enum class State
	{
		enInitialized,						// 初期化
		enConnecting,						// サーバー接続中
		enConnected,						// サーバーに接続済み
		enJoining,							// ルームにジョイン中
		enJoined,							// ルームにジョイン済み。
		enWaitRecvInitDataOtherPlayer,		// 開始データの受け取り待ち。
		enWaitStartGame,					// ゲームの開始待ち
		enInGameBufferingPadData,			// パッドデータのバッファリング中
		enInGame,							// ゲームプレイ中。
		enDisconnecting,					// サーバーから切断中。
		enDisconnected						// サーバーから切断済み。
	};

	State m_state = State::enInitialized;

	/**
	 * @brief イベント
	*/
	enum EnEvent {
		enEvent_SendInitDataForOtherPlayer,	  // 他プレイヤーの初期化情報を送る
		enEvent_PossibleGameStartOtherPlayer, // 他プレイヤーがゲーム開始可能になったことを通知
	};

	/**
	 * @brief sendDirect()関数で送られてくるメッセージの種類
	*/
	enum EnDirectMessageType {
		enDirectMessageType_PadData,				// パッドデータ
		enDirectMessageType_RequestResendPadData,	// パッドデータの再送リクエスト
	};
	
	/**
	 * @brief パッドデータ
	*/
	struct SPadData {
		int dataType;				// データの種類。
		XINPUT_STATE xInputState;	// XInputステート
		int frameNo;				// フレーム番号
		unsigned int checksum;		// チェックサム用のデータ。
	};

	/**
	 * @brief パッドデータの再送リクエスト
	*/
	struct SRequestResendPadData {
		int dataType;
		int frameNo;
	};

	/**
	 * @brief プレイヤーのネットワークタイプ
	*/
	enum EnPlayerType {
		enPlayerNetWorkType_host, // ホスト(部屋を作った)
		enPlayerNetWorkType_client, // クライアント(既存の部屋に入った)
		enPlayerNetWorkType_undef // 不明
	};

	EnPlayerType m_playerType = enPlayerNetWorkType_undef; // プレイヤーのタイプ
	
	/**
	 * @brief 他プレイヤーのネットワークの状態
	*/
	enum EnOtherPlayerState {
		enOtherPlayerNetWorkState_undef, // 不明
		enOtherPlayerNetWorkState_joinedRoom, // 部屋に入ってきた
		enOtherPlayerNetWorkState_possibleGameStart, // ゲーム開始可能状態
		enOtherPlayerNetWorkState_leftRoom // 部屋から抜けた
	};

	EnOtherPlayerState m_otherPlayerState = enOtherPlayerNetWorkState_undef; // 他プレイヤーの状態。


private: //data menber
    static NetWork* m_instance;		//唯一のインスタンスのアドレスを記録する変数。

	////////////////////////////////////////////////////////////
	// 関数のデータを保存する変数
	////////////////////////////////////////////////////////////

	// すべてのプレイヤーがルームに参加した時に呼ばれる関数を保存する変数の型の別名
	using OnAllPlayerJoinedRoom = std::function<void(void* pRecvData, int dataSize)>;
	// すべてのプレイヤーがゲームを開始できる状態になった時に呼ばれる関数を保存する変数の型の別名
	using OnAllPlayerNotifyPossibleGameStart = std::function<void()>;
	// エラーが発生した時に呼ばれる関数を保存する変数の型の別名
	using OnErrorFunc = std::function<void()>;

	// すべてのプレイヤーがルームに参加した時に呼ばれる関数を保存する変数
	OnAllPlayerJoinedRoom m_allPlayerJoinedRoomFunc = nullptr;
	// すべてのプレイヤーがゲーム開始可能であることを通知した時に呼ばれる関数を保存する変数
	OnAllPlayerNotifyPossibleGameStart m_allPlayerNotifyPossibleGameStartFunc = nullptr;
	// 通信エラーが起きた時に呼ばれる関数
	OnErrorFunc m_errorFunc = nullptr;

	////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////

	using LoadBalancingClientPtr = std::unique_ptr<ExitGames::LoadBalancing::Client>;

	LoadBalancingClientPtr m_loadBalancingClient;




	
	////////////////////////////////////////////////////////////
	// データを保存するポインタとデータのサイズ
	////////////////////////////////////////////////////////////

	std::unique_ptr<std::uint8_t[]> m_sendDataOnGameStart; // ゲーム開始時に転送するデータ。
	int m_sendDataSizeOnGameStart; // ゲーム開始時に転送するデータのサイズ。



	////////////////////////////////////////////////////////////
	// タイマー
	////////////////////////////////////////////////////////////

	float m_timer = 0.0f; // タイマー
	float m_waitLimitTime = 0.0f; // 待ちの最大秒数。


	////////////////////////////////////////////////////////////
	// フラグ
	////////////////////////////////////////////////////////////

	bool m_finishInit = false; // 初期化したか


	bool m_isPossibleGameStart = false; // ゲーム開始可能フラグ


	bool m_isHoge = false;



	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

	int m_frameNo = 0; // 現在のパッドのフレーム番号
	std::map< int, SPadData> m_padData[2]; // パッドデータ	
	int m_playFrameNo = 0; // 現在のゲーム進行フレーム番号
	GamePad m_pad[2]; // ゲームパッド

	int m_recieveDataSize = 0; // ゲーム開始のために受け取ったデータのサイズ

	std::unique_ptr<std::uint8_t[]> m_recieveDataOnGameStart; // ゲーム開始のために受け取ったデータ
};