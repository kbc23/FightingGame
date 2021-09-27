#pragma once

class ShadowMap
{
public:
	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new ShadowMap;
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
	static ShadowMap* GetInstance()
	{
		return m_instance;
	}

public:
    void CreateShadowMap();


public: //Get関数
	RenderTarget& GetShadowMap()
	{
		return m_shadowMap;
	}

	/// <summary>
	/// レンダリングターゲットを取得。
	/// </summary>
	/// <returns></returns>
	RenderTarget& GetRenderContext()
	{
		return m_shadowMap;
	}


private: //data menber
    static ShadowMap* m_instance;		//唯一のインスタンスのアドレスを記録する変数。

	RenderTarget m_shadowMap;
};