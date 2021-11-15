#include "stdafx.h"
#include "wav_file_bank.h"

#include "util.h"

WavFileBank::WavFileBank()
{

}

WavFileBank::~WavFileBank()
{
    ReleaseAll();
}

void WavFileBank::RegistWaveFile(int groupID, CWaveFilePtr waveFile)
{
	for (auto waveFile : m_waveFileMap[groupID]) {
		waveFile.second->Release();
	}
	m_waveFileMap[groupID].clear();
}

CWaveFilePtr WavFileBank::FindWaveFile(int groupID, const wchar_t* filePath)
{
	if (!(groupID < MAX_GROUP))
		MessageBoxA(nullptr, "groupID is invalid", "エラー", MB_OK);
	auto value = m_waveFileMap[groupID].find(Util::MakeHash(filePath));
	if (value != m_waveFileMap[groupID].end()) {
		return value->second;
	}
	return CWaveFilePtr();
}

//CWaveFilePtr WavFileBank::FindWaveFile(int groupID, const WNameKey& nameKey)
//{
//
//}

void WavFileBank::UnregistWaveFile(int groupID, CWaveFilePtr waveFile)
{
	if (!(groupID < MAX_GROUP))
		MessageBoxA(nullptr, "groupID is invalid", "エラー", MB_OK);
	m_waveFileMap->erase(waveFile->GetFilePathHash());
}

void WavFileBank::Release(int groupID)
{
	for (auto waveFile : m_waveFileMap[groupID]) {
		waveFile.second->Release();
	}
	m_waveFileMap[groupID].clear();
}