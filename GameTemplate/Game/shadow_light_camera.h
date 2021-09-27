#pragma once

class ShadowLightCamera
{
public:
	/// <summary>
	/// �C���X�^���X�̍쐬�B
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new ShadowLightCamera;
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
	static ShadowLightCamera* GetInstance()
	{
		return m_instance;
	}

public:
	void CreateShadowLightCamera();


public:
	Camera& GetShadowLightCamera()
	{
		return m_shadowLightCamera;
	}



private: //data menber
	static ShadowLightCamera* m_instance;		//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B

	Camera m_shadowLightCamera;
};