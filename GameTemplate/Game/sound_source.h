#pragma once
#include "wav_file.h"
#include "sound_engine.h"

class SoundSource : public IGameObject
{
public:
	SoundSource();
	~SoundSource();

	/**
	 * @brief 初期化
	 * @param filePath ファイルパス。対応しているファイルフォーマット(*.wav)
	 * @param is3DSound 3Dサウンドかどうか
	*/
	void Init(const wchar_t* filePath, bool is3DSound = false);

	/**
	 * @brief ストリーミング再生向けの初期化
	 * @param filePath ファイルパス。対応しているファイルフォーマット(*.wav)
	 * @param is3DSound 3Dサウンド？
	 * @param ringBufferSize リングバッファのサイズ。(bufferSizeの倍数になっていると無駄なく活用できる)
	 * @param bufferingSize ストリーミングの最大バッファリングサイズ
	*/
	void InitStreaming(
		wchar_t* filePath, bool is3DSound = false, unsigned int ringBufferSize = 3 * 1024 * 1024,
		unsigned int bufferingSize = 1024 * 512);

	/**
	 * @brief 開放
	*/
	void Release();

	/**
	 * @brief 再生
	 * @param isLoop ループ再生フラグ
	*/
	void Play(const bool isLoop);

	/**
	 * @brief 停止
	*/
	void Stop()
	{
		m_sourceVoice->Stop();
		m_isPlaying = false;
	}
	
	/**
	 * @brief 一時停止
	*/
	void Pause()
	{
		m_sourceVoice->Stop(/*XAUDIO2_PLAY_TAILS*/);
	}

	/**
	 * @brief 再生中か
	 * @return 再生中か
	*/
	const bool IsPlaying() const
	{
		return m_isPlaying;
	}

	/**
	 * @brief ボリュームを設定
	 * @param vol ボリューム
	*/
	void SetVolume(const float vol)
	{
		m_sourceVoice->SetVolume(vol);
	}

	/**
	 * @brief ボリュームを取得
	 * @return 現在のボリューム
	*/
	const float GetVolume() const
	{
		float vol;
		m_sourceVoice->GetVolume(&vol);
		return vol;
	}

	/**
	 * @brief 音源の座標を設定
	 * @details 3Dサウンドの時に必要。2Dサウンドでは無視。
	 * @param pos 音源の座標
	*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		if (m_isSetPositionFirst) {
			m_lastFramePosition = m_position;
			m_isSetPositionFirst = false;
		}
	}

	/**
	 * @brief 音源の座標を取得
	 * @return 音源の座標
	*/
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/**
	 * @brief 音源の移動速度を取得
	 * @return 音源の移動速度
	*/
	const Vector3& GetVelocity() const
	{
		return m_velocity;
	}

	/**
	 * @brief ループフラグを取得
	 * @return ループフラグ
	*/
	const bool GetLoopFlag() const
	{
		return m_isLoop;
	}

	/**
	 * @brief ボイスの周波数調整比
	 * @details 詳細はIXAudio2SourceVoiceのSetFrequencyRatioを参照
	 * @param ratio 周波数比
	*/
	void SetFrequencyRatio(float ratio)
	{
		if (m_sourceVoice != nullptr) {
			m_sourceVoice->SetFrequencyRatio(ratio);
		}
	}
	IXAudio2SourceVoice* GetXAudio2SourceVoice()
	{
		return m_sourceVoice;
	}
	//入力チャンネル数を取得。
	int GetNumInputChannel() const
	{
		return m_waveFile->GetFormat()->nChannels;
	}
	FLOAT32* GetEmitterAzimuths()
	{
		return m_emitterAzimuths;
	}
	FLOAT32* GetMatrixCoefficients()
	{
		return m_matrixCoefficients;
	}
	X3DAUDIO_DSP_SETTINGS* GetDspSettings()
	{
		return &m_dspSettings;
	}
private:
	void InitCommon();
	//ストリーミング再生中の更新処理。
	void UpdateStreaming();
	//オンメモリ再生中の更新処理。
	void UpdateOnMemory();

	void Play(char* buff, unsigned int bufferSize);

	/**
	 * @brief ストリーミングバッファリングの開始
	*/
	void StartStreamingBuffring();

	void Remove3DSound();

	/**
	 * @brief 更新
	*/
	void Update() override;


public: //Get関数
	/**
	 * @brief 曲の終わりにきたか
	 * @return 曲の終わりにきたか
	*/
	const bool GetEndOfSound() const
	{
		return m_endOfSound;
	}


private:
	enum EnStreamingStatus {
		enStreamingBuffering,	//バッファリング中。
		enStreamingQueueing,	//キューイング中。
	};
	std::shared_ptr<WavFile>	m_waveFile;					//!<波形データ。
	IXAudio2SourceVoice* m_sourceVoice = nullptr;	//!<ソースボイス。
	bool						m_isLoop = false;			//!<ループフラグ。
	bool						m_isPlaying = false;		//!<再生中フラグ。
	bool					m_isStreaming = false;		//!<ストリーミング再生？
	unsigned int			m_streamingBufferSize = 0;	//!<ストリーミング用のバッファリングサイズ。
	unsigned int			m_currentBufferingSize = 0;	//!<現在のバッファリングのサイズ。
	unsigned int			m_readStartPos = 0;			//!<読み込み開始位置。
	unsigned int			m_ringBufferSize = 0;		//!<リングバッファのサイズ。
	EnStreamingStatus		m_streamingState = enStreamingBuffering;	//!<ストリーミングステータス。
	bool					m_is3DSound = false;		//!<3Dサウンド？
	Vector3				m_position = Vector3::Zero;	//!<音源の座標。3Dサウンドの時に必要。
	Vector3				m_lastFramePosition = Vector3::Zero;//!<音源の1フレーム前の座標。3Dサウンドの時に必要。
	Vector3				m_velocity = Vector3::Zero;	//!<速度。3Dサウンドの時に必要・
	FLOAT32 m_emitterAzimuths[INPUTCHANNELS];
	FLOAT32 m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	X3DAUDIO_DSP_SETTINGS m_dspSettings;
	bool m_isSetPositionFirst = true;	//!<一番最初のsetPosition?
	bool m_isAvailable = false;			//!<インスタンスが利用可能？
	bool m_endOfSound = false; //曲の終わりにきたか
};