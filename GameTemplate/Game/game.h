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
	enum EnStep {
		enStep_InitNetWork,				// �l�b�g���[�N�̏������������Ȃ��B
		enStep_WaitAllPlayerJoined,		// ���ׂẴv���C���[�������܂ő҂B
		enStep_WaitAllPlayerStartGame,	// ���ׂẴv���C���[���Q�[���J�n����̂�҂B
		enStep_InGame,					// �C���Q�[���B
		enStep_Error,					// �G���[�B
	};

	EnStep m_step = enStep_InitNetWork;




private: //data menber
	ModelRender* m_modelCharacter[2] = { nullptr };	//�v���C���[�L�����N�^�[�̃��f��
    ModelRender* m_modelStage = nullptr;
    Shadow* m_shadowModelCharacter = nullptr;
	Actor* m_actor[2];

	float m_rotY = 0.0f;


    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity;		//��]




	int m_charaNo = 0;


    NetWork* m_onlineTwoPlayerMatchEngine = nullptr; // ���S������l�ΐ�p�G���W��

};