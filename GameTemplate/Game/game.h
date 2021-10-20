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
	// �l�b�g���[�N�֘A�̊֐�
	////////////////////////////////////////////////////////////
private:
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

	




private: //data menber
	////////////////////////////////////////////////////////////
	// �N���X�̃C���X�^���X
	////////////////////////////////////////////////////////////
	
	Player* m_player[2];
	NetWork* m_onlineTwoPlayerMatchEngine = nullptr; // ���S������l�ΐ�p�G���W��
	PlayerCamera* m_playerCamera = nullptr;

	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity; // ��]
	float m_rotY = 0.0f;

	int m_charaNo = 0; // �Ȃ񂩂���i�����I�������L�����N�^�[�̂��Ɓj

	int m_playerNum = -1;
	int m_otherPlayerNum = -1;

};