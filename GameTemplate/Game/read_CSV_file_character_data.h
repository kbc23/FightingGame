#pragma once
#include "read_CSV_file.h"

class ReadCSVFileCharacterData : public ReadCSVFile
{
public:
    /**
     * @brief csvファイルを開いてデータを取得
     * @param filePath 開くcsvファイルのパス
    */
    void ReadCSVFileData(std::string filePath);


public: // Get function
    /**
     * @brief キャラクターのモデルパスを取得
     * @return キャラクターのモデルパス
    */
    const std::string GetCharacterModelPath()
    {
        if (m_NOT_FOUND == m_characterModelPath) {
            MessageBoxA(nullptr, "キャラクターのモデルパスが見つかりませんでした", "エラー", MB_OK);
        }

        return m_characterModelPath;
    }
    /**
     * @brief キャラクターのアニメーションパスを取得
     * @param num アニメーションクリップ
     * @return キャラクターのアニメーションパス
    */
    const std::string GetCharacterAnimationPath(const int num)
    {
        if (m_NOT_FOUND == m_characterAnimationPath[num]) {
            MessageBoxA(nullptr, "キャラクターのアニメーションパスが見つかりませんでした", "エラー", MB_OK);
        }

        return m_characterAnimationPath[num];
    }


private: // data member
    std::string m_characterModelPath; // キャラクターのモデルパス
    std::vector<std::string> m_characterAnimationPath; // キャラクターのアニメーションパス
};