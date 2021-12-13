#pragma once
#include <fstream>
#include <sstream>

class ReadCSVFile
{
public:
	ReadCSVFile();
	ReadCSVFile(const std::string& openFile);
	~ReadCSVFile();

	////////////////////////////////////////////////////////////
	// ファイルの開閉関連
	////////////////////////////////////////////////////////////
	/**
	 * @brief ファイルを開く
	 * @param open_file 開くファイルの名前
	 * @return 開けたか
	*/
	const bool Open(const std::string& openFile);
	/**
	 * @brief ファイルを閉じる
	*/
	void Close();
	/**
	 * @brief ファイルを開いているかを確認する
	 * @return 開いているか
	*/
	const bool IsOpen()
	{
		return m_file.is_open();
	}
	/**
	 * @brief 保持しているファイルパスを取得
	 * @return 保持しているファイルパス
	*/
	const std::string GetOpenFilePath() const
	{
		// ファイルパスを保持していない場合
		if (true == m_filePath.empty()) {
			return "No file path is retained";
		}

		return m_filePath;
	}

	////////////////////////////////////////////////////////////
	// セルのデータの取得関連
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// 指定したセルのデータを取得
	//////////////////////////////
	/**
	 * @brief 指定したセルのデータを取得する
	 * @param x セルのX座標
	 * @param y セルのY座標
	 * @return 取得したセルのデータ
	*/
	std::string GetCell(const int xCell, const int yCell);

	//////////////////////////////
	// 指定したキーワードがある行のすべてのセルのデータを取得
	//////////////////////////////

	/**
	 * @brief 上記のオーバーライド(vector<std::string>)
	 * @param check_word キーワード
	 * @param array_str 取得したセルのデータを入れる配列
	*/
	void GetLineByWord(const std::string& checkWord, std::vector<std::string>& arrayStr);


protected: //constant
	static std::string m_NOT_FOUND; // データが見つからなかったときに入力する文字列


private: // data member
    std::ifstream m_file; // 開いたファイルデータを保持する変数
	std::string m_filePath; // 開いているファイルのファイルパス
};