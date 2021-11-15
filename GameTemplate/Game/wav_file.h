#pragma once
#include "util.h"

class WavFile
{
public:
    WavFile();
    ~WavFile();

	/**
	 * @brief wavファイルを開く
	 * @param filePath ファイル名
	 * @return wavファイルを開けたか
	*/
	bool Open(const wchar_t* filePath);

	/**
	 * @brief 波形データを読み込み
	 * @param pBuffer 波形データを書き込むバッファ
	 * @param sizeToRead 読み込むデータサイズ
	 * @param currentReadSize 
	*/
	void Read(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);

	/**
	 * @brief 波形データを非同期読み込み。
	 * @param pBuffer 波形データを書き込むバッファ。
	 * @param sizeToRead 読み込むデータサイズ。
	 * @param currentReadSize 
	*/
	void ReadAsync(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);


	/**
	 * @brief 波形データのシーク位置をリセット
	*/
	void ResetFile();

	/**
	 * @brief リリース
	*/
	void Release();


public: //Get関数

	/**
	 * @brief 読み込み終了判定
	 * @return 
	*/
	const bool IsReadEnd() const
	{
		return m_isReadEnd;
	}

	/**
	 * @brief ファイルサイズを取得
	 * @return 
	*/
	DWORD GetSize() const
	{
		return m_dwSize;
	}

	/**
	 * @brief ファイルフォーマットを取得
	 * @return 
	*/
	WAVEFORMATEX* GetFormat() const
	{
		return m_pwfx;
	};

	/**
	 * @brief ファイルパスのハッシュ値を取得
	 * @return 
	*/
	unsigned int GetFilePathHash() const
	{
		return m_filePathHash;
	}

	/**
	 * @brief 読み込み先のバッファを確保
	 * @param size 
	*/
	void AllocReadBuffer(int size)
	{
		m_readBuffer.reset(new char[size]);
	}

	/**
	 * @brief 読み込み先のバッファを取得
	 * @return 
	*/
	char* GetReadBuffer()
	{
		return m_readBuffer.get();
	}

private:
	std::unique_ptr<char[]>	m_readBuffer = nullptr;	//読み込み先のバッファ。
	HMMIO			m_hmmio = nullptr;	//Waveファイルハンドル。
	WAVEFORMATEX* m_pwfx = nullptr;  //waveファイルのフォーマット定義。
	MMCKINFO 		m_ckRiff;      // Use in opening a WAVE file
	DWORD			m_dwSize = 0;      // The size of the wave file
	MMCKINFO		m_ck;          // Multimedia RIFF chunk
	BYTE* m_pbData = nullptr;
	BYTE* m_pbDataCur = nullptr;
	ULONG			m_ulDataSize = 0;
	volatile bool	m_isReadEnd = true;	//読み込み終了フラグ。
	std::thread		m_readAsyncThread;	//非同期読み込みスレッド。
	bool			m_isInvokeReadAsyncThread = false;
	std::wstring	m_filePath;			//ファイルパス。
	unsigned int	m_filePathHash = 0;		//ファイルパスのハッシュコード。

};