#pragma once

class ShadowLightCamera
{
public:
	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new ShadowLightCamera;
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
	static ShadowLightCamera* m_instance;		//唯一のインスタンスのアドレスを記録する変数。

	Camera m_shadowLightCamera;
};