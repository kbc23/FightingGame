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
	// �Q�[�����̏���
	////////////////////////////////////////////////////////////

	void UpdateGame();

	void InGame();

	void FinishGame();

	//////////////////////////////
	// �Q�[���I������
	//////////////////////////////

	void CheckGameEnd();

	const bool CheckHp_0();

	////////////////////////////////////////////////////////////
	// �l�b�g���[�N�֘A�̊֐�
	////////////////////////////////////////////////////////////
	/// <summary>
    /// ���ׂẴv���C���[�����[���ɃW���C�������Ƃ��ɌĂ΂�鏈���B
    /// </summary>
    /// <param name="pData"></param>
    /// <param name="size"></param>
	void OnAllPlayerJoined(void* pData, int size);
	/// <summary>
	/// ���ׂẴv���C���[���Q�[���X�^�[�g�������ɌĂ΂�鏈���B
	/// </summary>
	void OnAllPlayerStartGame();
	/// <summary>
	/// �ʐM�G���[���N�����B
	/// </summary>
	void OnError();

	



private: //enum
	enum class EnStep {
		enStep_InitNetWork,				// �l�b�g���[�N�̏������������Ȃ��B
		enStep_WaitAllPlayerJoined,		// ���ׂẴv���C���[�������܂ő҂B
		enStep_WaitAllPlayerStartGame,	// ���ׂẴv���C���[���Q�[���J�n����̂�҂B
		enStep_InGame,					// �C���Q�[���B
		enStep_Error,					// �G���[�B
	};

	EnStep m_step = EnStep::enStep_InitNetWork;

	enum class EnGameStatus
	{
		enInGame, // �Q�[����
		enFinishGame // �Q�[���I��
	};

	EnGameStatus m_gameStatus = EnGameStatus::enInGame;

	/**
	 * @brief ���s
	*/
	enum EnWinOrLose
	{
		enWin, // ����
		enLose, // ����
		enInTheGame // ������
	};

	EnWinOrLose m_winOrLose[2] = { enInTheGame,enInTheGame };


private: //data menber
	////////////////////////////////////////////////////////////
	// �N���X�̃C���X�^���X
	////////////////////////////////////////////////////////////
	
	Player* m_player[2];
	NetWork* m_onlineTwoPlayerMatchEngine = nullptr; // ���S������l�ΐ�p�G���W��
	PlayerCamera* m_playerCamera = nullptr;
	GameData* m_gameData = nullptr;
	FontRender* m_fontWinOrLose = nullptr;
	ModelRender* m_modelStage = nullptr; // �X�e�[�W�̃��f��

	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity; // ��]
	float m_rotY = 0.0f;

	int m_charaNo = 0; // �Ȃ񂩂���i�����I�������L�����N�^�[�̂��Ɓj

	int m_playerNum = -1;
	int m_otherPlayerNum = -1;



	int m_debugCountGameEnd = 0;
};