#pragma once
#include "sound_source.h"

class SoundSE : public IGameObject
{
public:
    SoundSE();
    ~SoundSE();

    /**
     * @brief 初期化
     * @param filePath wavファイルパス
     * @param is3DSound 3D再生するか
    */
    void Init(const wchar_t* filePath, const bool is3DSound = false);

    /**
     * @brief 再生
     * @param isLoop ループ再生するか
    */
    void Play(const bool isLoop);

    /**
     * @brief ボリュームを設定（1.0fが通常）
     * 負の値を入れると絶対値になる。
     * @param volume ボリューム
    */
    void SetVolume(const float volume)
    {
        //ボリュームを保持
        m_volume = volume;

        //ループ再生用のサウンドソースが使われていないなら
        if (m_loopSoundSource == nullptr) {
            return;
        }

        //ループ再生用のサウンドソースのボリュームを設定する
        m_loopSoundSource->SetVolume(m_volume * m_SEVolume);
    }

    /**
     * @brief SE全体のボリュームを設定
     * @param BGMVolume SE全体のボリュームを設定
    */
    void SetSEVolume(const float BGMVolume)
    {
        m_SEVolume = BGMVolume;

        //ループ再生用のサウンドソースが使われていないなら
        if (m_loopSoundSource == nullptr) {
            return;
        }

        //ループ再生用のサウンドソースのボリュームを設定する
        m_loopSoundSource->SetVolume(m_volume * m_SEVolume);
    }

    /**
     * @brief ボリュームを取得
     * @return ボリューム
    */
    const float GetVolume() const
    {
        return m_volume;
    }

    /**
     * @brief 再生を停止
     * 次に再生するときは最初から
    */
    void Stop()
    {
        //ループ再生用のサウンドソースが使われていたら
        if (!m_loopSoundSource) {
            return;
        }

        //サウンドを停止する
        m_loopSoundSource->Stop();
    }

    /**
     * @brief 再生を一時停止
     * 次に再生するときは一時停止した地点から
    */
    void Pause()
    {
        //ループ再生用のサウンドソースが使われていたら
        if (!m_loopSoundSource) {
            return;
        }

        //サウンドを一時停止する
        m_loopSoundSource->Pause();
        //フラグを一時停止中にする
        m_isPaused = true;
    }

    /**
     * @brief 再生中か
     * @return 再生中か
    */
    const bool IsPlaying() const
    {
        //ループ再生用のサウンドソースが使われていたら
        if (!m_loopSoundSource) {
            return false;
        }

        //再生中かどうか調べる
        return m_loopSoundSource->IsPlaying();
    }

    /**
     * @brief 一時停止中か
     * @return 一時停止中か
    */
    const bool IsPaused() const
    {
        return m_isPaused;
    }

    /**
     * @brief 曲の終わりにきたか
     * @return 曲の終わりにきたか
    */
    const bool EndOfSound() const
    {
        return m_loopSoundSource->GetEndOfSound();
    }

private:	//privateなメンバ関数
    /**
     * @brief ループ再生の処理
    */
    void PlayLoop();

    /**
     * @brief ワンショット再生の処理
    */
    void PlayOneShot();


private: //data menber
    wchar_t m_filePath[256];					//wavファイルパス
    bool m_is3DSound = false;					//3Dサウンドで再生するか
    float m_volume = 1.0f;						//全体のボリューム
    bool m_isPaused = false;					//一時停止中か
    SoundSource* m_loopSoundSource = nullptr;	//ループ再生用のサウンドソース


private: //static data menber
    static float m_SEVolume;
};