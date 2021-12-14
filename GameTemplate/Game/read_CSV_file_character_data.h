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
        // ファイルネームが見つかっていない場合
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
        if (m_characterAnimationPath.size() <= num) {
            MessageBoxA(nullptr, "キャラクターのアニメーションの初期化時に、配列の要素外にアクセスしようとしました", "プログラムエラー", MB_OK);
            MessageBoxA(nullptr, "ゲームを終了します", "メッセージ", MB_OK);

            //ゲームを終了
            exit(EXIT_SUCCESS);
        }

        // ファイルネームが見つかっていない場合
        if (m_NOT_FOUND == m_characterAnimationPath[num]) {
            MessageBoxA(nullptr, "キャラクターのアニメーションパスが見つかりませんでした", "エラー", MB_OK);
        }

        return m_characterAnimationPath[num];
    }


private: // data member
    std::string m_characterModelPath; // キャラクターのモデルパス
    std::vector<std::string> m_characterAnimationPath; // キャラクターのアニメーションパス
};