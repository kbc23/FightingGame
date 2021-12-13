#include "stdafx.h"

#include <iostream>
#include <vector>

#include "read_CSV_file_character_data.h"


void ReadCSVFileCharacterData::ReadCSVFileData(std::string filePath)
{
    ReadCSVFile readCSVFile;

    readCSVFile.Open(filePath);

    m_characterModelPath = readCSVFile.GetCell(2, 1);

    readCSVFile.GetLineByWord("アニメーション", m_characterAnimationPath);

    readCSVFile.Close();

    for (int test = 0; m_characterAnimationPath.size() > test; ++test) {
        if ("アニメーション" == m_characterAnimationPath[test]) {
            m_characterAnimationPath.erase(m_characterAnimationPath.begin() + test);
            break;
        }
    }
}