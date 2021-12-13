#pragma once
#include "read_CSV_file.h"

class ReadCSVFileCharacterData : public ReadCSVFile
{
public:
    /**
     * @brief csv�t�@�C�����J���ăf�[�^���擾
     * @param filePath �J��csv�t�@�C���̃p�X
    */
    void ReadCSVFileData(std::string filePath);


public: // Get function
    /**
     * @brief �L�����N�^�[�̃��f���p�X���擾
     * @return �L�����N�^�[�̃��f���p�X
    */
    const std::string GetCharacterModelPath()
    {
        if (m_NOT_FOUND == m_characterModelPath) {
            MessageBoxA(nullptr, "�L�����N�^�[�̃��f���p�X��������܂���ł���", "�G���[", MB_OK);
        }

        return m_characterModelPath;
    }
    /**
     * @brief �L�����N�^�[�̃A�j���[�V�����p�X���擾
     * @param num �A�j���[�V�����N���b�v
     * @return �L�����N�^�[�̃A�j���[�V�����p�X
    */
    const std::string GetCharacterAnimationPath(const int num)
    {
        if (m_NOT_FOUND == m_characterAnimationPath[num]) {
            MessageBoxA(nullptr, "�L�����N�^�[�̃A�j���[�V�����p�X��������܂���ł���", "�G���[", MB_OK);
        }

        return m_characterAnimationPath[num];
    }


private: // data member
    std::string m_characterModelPath; // �L�����N�^�[�̃��f���p�X
    std::vector<std::string> m_characterAnimationPath; // �L�����N�^�[�̃A�j���[�V�����p�X
};