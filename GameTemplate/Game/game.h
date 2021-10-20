#pragma once
#include "model_render.h"
#include "shadow.h"
#include "net_work.h"

class PlayerCamera;
class Actor;
class Player;

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Update() override final;


	////////////////////////////////////////////////////////////
	// ネットワーク関連の関数
	////////////////////////////////////////////////////////////
private:
	/// <summary>
    /// すべてのプレイヤーがルームにジョインしたときに呼ばれる処理。
    /// </summary>
    /// <param name="pData"></param>
    /// <param name="size"></param>
	void OnAllPlayerJoined(void* pData, int size);
	/// <summary>
	/// すべてのプレイヤーがゲームスタートした時に呼ばれる処理。
	/// </summary>
	void OnAllPlayerStartGame();
	/// <summary>
	/// 通信エラーが起きた。
	/// </summary>
	void OnError();


private: //enum
	enum class EnStep {
		enStep_InitNetWork,				// ネットワークの初期化をおこなう。
		enStep_WaitAllPlayerJoined,		// すべてのプレイヤーが揃うまで待つ。
		enStep_WaitAllPlayerStartGame,	// すべてのプレイヤーがゲーム開始するのを待つ。
		enStep_InGame,					// インゲーム。
		enStep_Error,					// エラー。
	};

	EnStep m_step = EnStep::enStep_InitNetWork;

	




private: //data menber
	////////////////////////////////////////////////////////////
	// クラスのインスタンス
	////////////////////////////////////////////////////////////
	
	Player* m_player[2];
	NetWork* m_onlineTwoPlayerMatchEngine = nullptr; // 完全同期二人対戦用エンジン
	PlayerCamera* m_playerCamera = nullptr;

	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity; // 回転
	float m_rotY = 0.0f;

	int m_charaNo = 0; // なんかある（多分選択したキャラクターのこと）

	int m_playerNum = -1;
	int m_otherPlayerNum = -1;

};