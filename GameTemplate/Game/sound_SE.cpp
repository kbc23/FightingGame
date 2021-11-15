#include "stdafx.h"
#include "sound_SE.h"

float SoundSE::m_SEVolume = 1.0f;

SoundSE::SoundSE()
{

}

SoundSE::~SoundSE()
{
	//CSoundCueオブジェクトをDeleteGOした際、再生中だった場合、停止させる。
	if (m_loopSoundSource) {
		DeleteGO(m_loopSoundSource);
	}
}

void SoundSE::Init(const wchar_t* filePath, const bool is3DSound)
{
	swprintf_s(m_filePath, filePath);

	//一度、作って破棄することで、
	//Flyweightパターンを使用しているWaveFileBankに
	//登録させる。
	SoundSource* ss = NewGO<SoundSource>(0);
	ss->Init(m_filePath, m_is3DSound);
	DeleteGO(ss);
}

void SoundSE::Play(const bool isLoop)
{
	if (isLoop)
	{
		//ループ再生
		PlayLoop();
	}
	else
	{
		//ワンショット再生
		PlayOneShot();
	}
}

void SoundSE::PlayLoop()
{
	//ループ再生用のサウンドソースが
//すでに作れれていたら
	if (m_loopSoundSource)
	{
		//一時停止したものを再開する
		m_loopSoundSource->Play(true);
		//フラグを一時停止中から解除する
		m_isPaused = false;
	}
	else
	{
		//ループ再生用のサウンドソースが
		//まだ未使用の場合は
		//新しく作って再生する。
		m_loopSoundSource = NewGO<SoundSource>(0);
		m_loopSoundSource->Init(m_filePath, m_is3DSound);
		m_loopSoundSource->Play(true);
		SetVolume(m_volume);
	}
}

void SoundSE::PlayOneShot()
{
	//毎回、サウンドソースを作って再生させる
	//Flyweightパターンを使用しているから重くならない。
	SoundSource* ss = NewGO<SoundSource>(0);
	ss->Init(m_filePath, m_is3DSound);
	ss->SetVolume(m_volume * m_SEVolume);	//OSの場合はここでボリュームを設定する
	ss->Play(false);
}