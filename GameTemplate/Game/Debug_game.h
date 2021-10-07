#pragma once

class DebugGame
{
public:
	/// <summary>
	/// インスタンスの作成。
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new DebugGame;
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
	static DebugGame* GetInstance()
	{
		return m_instance;
	}


public:
	/**
	 * @brief デバッグモードか
	 * @return デバッグモードかどうか
	*/
	const bool DebugMode()
	{
		return m_debugMode;
	}
    /**
     * @brief デバッグのソロモードか
     * @return デバッグのソロモードかどうか
    */
    const bool DebugSoloMode()
    {
		if (false == m_debugMode) {
			return false;
		}

        return m_debugSoloMode;
    }



public:
	/**
	 * @brief デバッグに設定
	*/
	void OnDebugSoloMode()
	{
		m_debugMode = true;
	}
	/**
	 * @brief デバッグでなくする
	*/
	void OffDebugSoloMode()
	{
		m_debugMode = false;
	}
	/**
	 * @brief デバッグのソロモードに設定
	*/
	void OnDebugSoloMode()
	{
		if (false == m_debugMode) {
			return;
		}

		m_debugSoloMode = true;
	}
	/**
	 * @brief デバッグのソロモードでなくする
	*/
	void OffDebugSoloMode()
	{
		if (false == m_debugMode) {
			return;
		}

		m_debugSoloMode = false;
	}



private: // data menber

	static DebugGame* m_instance;		//唯一のインスタンスのアドレスを記録する変数

	////////////////////////////////////////////////////////////
	// フラグ
	////////////////////////////////////////////////////////////

	bool m_debugMode = false; // デバッグモードか
    bool m_debugSoloMode = false; // デバッグのソロモードか





};