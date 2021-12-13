#include "stdafx.h"

#include <iostream>
#include <vector>

#include "read_CSV_file_character_data.h"


void ReadCSVFileCharacterData::ReadCSVFileData(std::string filePath)
{
    // ファイルを開く
    Open(filePath);
    
    // モデルアセットのフォルダーのパスを取得
    std::string fileFolderPath = GetCell(2, 1);
    // キャラクターモデルのファイルネームを取得
    m_characterModelPath = GetCell(2, 2);
    // アニメーションデータのファイルネームを取得
    GetLineByWord("アニメーション", m_characterAnimationPath);

    // ファイルを閉じる
    Close();

    // 必要ない情報を削除
    for (int animationNum = 0; m_characterAnimationPath.size() > animationNum; ++animationNum) {
        if ("アニメーション" == m_characterAnimationPath[animationNum]) {
            m_characterAnimationPath.erase(m_characterAnimationPath.begin() + animationNum);
            break;
        }
    }

    ////////////////////////////////////////////////////////////
    // ファイルパスをファイルネームの前に付ける
    ////////////////////////////////////////////////////////////

    // ファイルパスをファイルネームの前に付ける
    m_characterModelPath = fileFolderPath + m_characterModelPath;
    // ファイルパスをファイルネームの前に付ける
    for (int animationNum = 0; m_characterAnimationPath.size() > animationNum; ++animationNum) {
        m_characterAnimationPath[animationNum] = fileFolderPath + m_characterAnimationPath[animationNum];
    }
}