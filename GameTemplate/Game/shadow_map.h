#pragma once

class ShadowMap
{
public:
	/// <summary>
	/// �C���X�^���X�̍쐬�B
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new ShadowMap;
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
	static ShadowMap* GetInstance()
	{
		return m_instance;
	}

public:
    void CreateShadowMap();


public: //Get�֐�
	RenderTarget& GetShadowMap()
	{
		return m_shadowMap;
	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g���擾�B
	/// </summary>
	/// <returns></returns>
	RenderTarget& GetRenderContext()
	{
		return m_shadowMap;
	}


private: //data menber
    static ShadowMap* m_instance;		//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B

	RenderTarget m_shadowMap;
};