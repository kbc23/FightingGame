#pragma once
#include "read_CSV_file.h"

class ReadCSVFileCharacterData : public ReadCSVFile
{
public:
    void ReadCSVFileData(std::string filePath);

    const std::string GetCharacterModelPath()
    {
        return m_characterModelPath;
    }

    const std::string GetCharacterAnimationPath(const int num)
    {
        return m_characterAnimationPath[num];
    }

private: // data member
    std::string m_characterModelPath;
    std::vector<std::string> m_characterAnimationPath;
};