#pragma once
#include "wav_file_bank.h"
#include "wav_file.h"

#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//最大出力チャンネル数。

class SoundSource;

class SoundEngine
{
public:
	/*!
	 * @brief コンストラクタ
	 */
	SoundEngine();

	/*!
	 * @brief デストラクタ
	 */
	~SoundEngine();

	/*!
	 * @brief 初期化
	 */
	void Init();

	/*!
	 * @brief 開放
	 */
	void Release();

	/*!
	* @brief 更新
	*/
	void Update();
	
	/**
	 * @brief 3Dサウンドソースを追加
	 * @param source 追加する3Dサウンドソース
	*/
	void Add3DSoundSource(SoundSource* source)
	{
		m_3dSoundSource.push_back(source);
	}

	/**
	 * @brief 3Dサウンドソースを削除
	 * @param source 削除する3Dサウンドソース
	*/
	void Remove3DSoundSource(SoundSource* source)
	{
		auto it = std::find(m_3dSoundSource.begin(), m_3dSoundSource.end(), source);
		if (it != m_3dSoundSource.end()) {
			m_3dSoundSource.erase(it);
		}
	}

	/**
	 * @brief サウンドリスナーの座標を設定
	 * @details 3D音源を使っている時の計算に使用
	 * @param pos サウンドリスナーの座標
	*/
	void SetListenerPosition(const Vector3& pos)
	{
		m_listenerPosition = pos;
	}

	/**
	 * @brief サウンドリスナーの前方向を設定
	 * @details 3D音源を使っている時の計算に使用
	 * @param front サウンドリスナーの前方向
	*/
	void SetListenerFront(const Vector3& front)
	{
		m_listener.OrientFront.x = front.x;
		m_listener.OrientFront.y = front.y;
		m_listener.OrientFront.z = front.z;
	}

	/**
	 * @brief サウンドリスナーの上方向を設定
	 * 	 * @details 3D音源を使っている時の計算に使用
	 * @param up サウンドリスナーの上方向
	*/
	void SetListenerUp(const Vector3& up)
	{
		m_listener.OrientTop.x = up.x;
		m_listener.OrientTop.y = up.y;
		m_listener.OrientTop.z = up.z;
	}
	/*!
	* @brief	XAudio2のソースボイスを作成。
	*/
	IXAudio2SourceVoice* CreateXAudio2SourceVoice(WavFile* waveFile, bool is3DSound);
	IXAudio2MasteringVoice* GetMasteringVoice()
	{
		return m_masteringVoice;
	}
	IXAudio2SubmixVoice* GetSubmixVoice()
	{
		return m_submixVoice;
	}
	/*!
	* @brief	出力チャンネルの数を取得。
	*/
	const int GetNumChannel() const
	{
		return m_nChannels;
	}
	/*!
	* @brief	波形データバンクを取得。
	*/
	WavFileBank& GetWaveFileBank()
	{
		return m_waveFileBank;
	}
	/// <summary>
	/// サウンドエンジンが利用可能かどうか判定。
	/// </summary>
	/// <returns>trueが返ってきたら利用可能です。</returns>
	const bool IsAvailable() const
	{
		return m_isInited;
	}


private:	//staticなデータメンバ
		static SoundEngine* m_instance;


public:		//staticなメンバ関数
		/// <summary>
	/// シングルトンパターン
	/// 唯一のインスタンスを作る関数
	/// 最初に呼んでね！
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new SoundEngine;
	}

	/// <summary>
	/// インスタンスを消す関数
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}

	/// <summary>
	/// インスタンスの参照を戻す関数
	/// </summary>
	/// <returns>インスタンスの参照</returns>
	static SoundEngine* GetInstance()
	{
		return m_instance;
	}


private:
	IXAudio2* m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;
	X3DAUDIO_HANDLE m_hx3DAudio;					//!<3Dオーディオのハンドル。
	IUnknown* m_reverbEffect = nullptr;				//!<リバープエフェクト。
	IXAudio2SubmixVoice* m_submixVoice = nullptr;	//!<サブミックスボイス。
	DWORD m_channelMask = 0;						//!<オーディオチャンネルマスク。
	DWORD m_nChannels = 0;							//!<チャンネル数。
	Vector3 m_listenerPosition = Vector3::Zero;	//!<リスナーの座標。
	float m_fListenerAngle = 0.0f;
	bool m_UseListenerCone = false;
	bool m_fUseInnerRadius = false;
	bool m_useRedirectToLFE = false;
	bool m_isInited = false;
	X3DAUDIO_LISTENER m_listener;					//!<サウンドリスナー。
	std::list<SoundSource*>	m_3dSoundSource;	//!<3Dサウンドのサウンドソース。
	X3DAUDIO_CONE	m_emitterCone;
	WavFileBank m_waveFileBank;					//!<波形データのバンク。
};