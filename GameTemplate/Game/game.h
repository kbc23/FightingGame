#pragma once
#include "model_render.h"
#include "shadow.h"
#include "net_work.h"

class Actor;

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Update() override final;


private:
	void NetWorkStart();





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
	enum EnStep {
		enStep_InitNetWork,				// ネットワークの初期化をおこなう。
		enStep_WaitAllPlayerJoined,		// すべてのプレイヤーが揃うまで待つ。
		enStep_WaitAllPlayerStartGame,	// すべてのプレイヤーがゲーム開始するのを待つ。
		enStep_InGame,					// インゲーム。
		enStep_Error,					// エラー。
	};

	EnStep m_step = enStep_InitNetWork;




private: //data menber
	ModelRender* m_modelCharacter[2] = { nullptr };	//プレイヤーキャラクターのモデル
    ModelRender* m_modelStage = nullptr;
    Shadow* m_shadowModelCharacter = nullptr;
	Actor* m_actor[2];

	float m_rotY = 0.0f;


    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity;		//回転




	int m_charaNo = 0;


    NetWork* m_onlineTwoPlayerMatchEngine = nullptr; // 完全同期二人対戦用エンジン

};