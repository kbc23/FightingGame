#pragma once
#include "model_render.h"
#include "shadow.h"
#include "net_work.h"
#include "font_render.h"

class PlayerCamera;
class Player;
class GameData;

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Update() override final;


private:
	////////////////////////////////////////////////////////////
	// ゲーム中の処理
	////////////////////////////////////////////////////////////

	void UpdateGame();

	void InGame();

	void FinishGame();

	//////////////////////////////
	// ゲーム終了判定
	//////////////////////////////

	void CheckGameEnd();

	const bool CheckHp_0();

	////////////////////////////////////////////////////////////
	// ネットワーク関連の関数
	////////////////////////////////////////////////////////////
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

	enum class EnGameStatus
	{
		enInGame, // ゲーム中
		enFinishGame // ゲーム終了
	};

	EnGameStatus m_gameStatus = EnGameStatus::enInGame;

	/**
	 * @brief 勝敗
	*/
	enum EnWinOrLose
	{
		enWin, // 勝利
		enLose, // 負け
		enInTheGame // 試合中
	};

	EnWinOrLose m_winOrLose[2] = { enInTheGame,enInTheGame };


private: //data menber
	////////////////////////////////////////////////////////////
	// クラスのインスタンス
	////////////////////////////////////////////////////////////
	
	Player* m_player[2];
	NetWork* m_onlineTwoPlayerMatchEngine = nullptr; // 完全同期二人対戦用エンジン
	PlayerCamera* m_playerCamera = nullptr;
	GameData* m_gameData = nullptr;
	FontRender* m_fontWinOrLose = nullptr;
	ModelRender* m_modelStage = nullptr; // ステージのモデル

	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity; // 回転
	float m_rotY = 0.0f;

	int m_charaNo = 0; // なんかある（多分選択したキャラクターのこと）

	int m_playerNum = -1;
	int m_otherPlayerNum = -1;



	int m_debugCountGameEnd = 0;
};