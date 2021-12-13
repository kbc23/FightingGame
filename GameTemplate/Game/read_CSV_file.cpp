#include "stdafx.h"
#include "read_CSV_file.h"

//////////////////////////////
// std::getline(A, B, C);
// A:文字を抜き出される方
// B:抜き出した文字をぶち込む方
// C:抜き出す際の区切りになるやつ
//////////////////////////////



namespace con
{
	const int FIRST_OF_THE_ARRAY = 0; // 配列の最初の要素
	const int FIRST_OF_THE_READING_POSITION = 0; // 読み取り位置の最初の位置
	const int NG_LESS_THAN_CELL_SPECIFICATION = 0; // セルの指定のNGの値（以下）

	const std::string EMPTY_CELL = ""; // 空のセル
	const char CELL_DELIMITER = ','; // セルの区切り
}

std::string ReadCSVFile::m_NOT_FOUND = "not found"; // データが見つからなかったときに入力する文字列

ReadCSVFile::ReadCSVFile()
{
	// TODO: 何しよう？
}

ReadCSVFile::ReadCSVFile(const std::string& openFile)
{
	// ファイルパスを保持
	m_filePath = openFile;
	// ファイルを開く
	m_file.open(m_filePath);

	// ファイルが開けなかったときの処理
	if (!m_file) {
		// TODO: ここにファイルが開けなかったときの処理を書きたいけど、
		// 何を書いたらいいか全然思いつかない。
	}
}

ReadCSVFile::~ReadCSVFile()
{
	// ファイルを開いている場合、ファイルを閉じる
	if (true == m_file.is_open()) {
		m_file.close();
	}
}

////////////////////////////////////////////////////////////
// ファイルの開閉関連
////////////////////////////////////////////////////////////

const bool ReadCSVFile::Open(const std::string& openFile)
{
	// すでに開いている
	if (true == IsOpen()) {
		return false;
	}

	// ファイルパスを保持
	m_filePath = openFile;
	// ファイルを開く
	m_file.open(m_filePath);

	// ファイルが開けなかったときの処理
	if (!m_file) {
		// TODO: ここにファイルが開けなかったときの処理を書きたいけど、
		// 何を書いたらいいか全然思いつかない。
		return false;
	}

	return true;
}

void ReadCSVFile::Close()
{
	// ファイルを開いていない場合、処理を終了
	if (false == IsOpen()) {
		return;
	}

	// ファイルを閉じる
	m_file.close();
	// ファイルパスを破棄
	m_filePath.clear();
}

////////////////////////////////////////////////////////////
// セルのデータの取得関連
////////////////////////////////////////////////////////////

std::string ReadCSVFile::GetCell(const int xCell, const int yCell)
{
	if (false == IsOpen()) {
		return m_NOT_FOUND;
	}

	// セルの指定で０以下を指定している場合
	if (con::NG_LESS_THAN_CELL_SPECIFICATION >= xCell ||
		con::NG_LESS_THAN_CELL_SPECIFICATION >= yCell) {
		return m_NOT_FOUND;
	}

	std::string getlineYStr; // Y軸の行のgetline()で使用する変数
	std::string cellStr; // 取得したセルの文字列を保管する変数

	// 指定した行を検索
	for (int checkYNum = con::FIRST_OF_THE_ARRAY; checkYNum < yCell; checkYNum++) {
		// 指定した行が見つからない場合
		if (true == m_file.eof()) {
			// csvファイルの読み取り位置をファイルの先頭に移動する
			m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

			return m_NOT_FOUND;
		}

		// 行のデータを取得
		std::getline(m_file, getlineYStr);
	}

	std::stringstream YLineStr(getlineYStr); // 取得した行のデータを保管する変数

	// 見つけた行から指定したセルを検索
	for (int checkCellNum = con::FIRST_OF_THE_ARRAY; checkCellNum < xCell; checkCellNum++) {
		// 指定したセルが見つからない場合
		if (true == YLineStr.eof()) {
			// csvファイルの読み取り位置をファイルの先頭に移動する
			m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

			return m_NOT_FOUND;
		}

		// セルのデータを取得
		std::getline(YLineStr, cellStr, con::CELL_DELIMITER);
	}

	// csvファイルの読み取り位置をファイルの先頭に移動する
	m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

	// セルのデータが空の場合
	if (con::EMPTY_CELL == cellStr) {
		return m_NOT_FOUND;
	}

	return cellStr;
}

void ReadCSVFile::GetLineByWord(const std::string& checkWord, std::vector<std::string>& arrayStr)
{
	if (false == IsOpen()) {
		arrayStr.push_back(m_NOT_FOUND);

		return;
	}

	std::string getlineYStr; // Y軸の行のgetline()で使用する変数
	std::string cellStr; // 取得したセルの文字列を保管する変数
	bool flagWordMatch = false; // 指定した文字列と同じ文字列があったか

	// 指定したキーワードと一致するセルがある行を検索
	while (std::getline(m_file, getlineYStr)) {
		// 最初にcsvファイルの一文を抜き出す
		// getlineで抜き出せるようにするためにstringstream型の変数に入れる
		std::stringstream YLineStr(getlineYStr); // 取得した行のデータを保管する変数

		// 一致する文字列があるか検索
		while (std::getline(YLineStr, cellStr, con::CELL_DELIMITER)) {
			// 一致する文字列があった場合
			if (checkWord == cellStr) {
				flagWordMatch = true;

				break;
			}
		}

		// 一致する文字列があった場合
		if (true == flagWordMatch) {
			//std::stringstream r_str(getlineYStr);

			// 文字列の先頭の位置にカーソルを移動
			YLineStr.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

			// vector配列に格納する
			while (std::getline(YLineStr, cellStr, con::CELL_DELIMITER)) {
				// データが空のセルがあった場合
				if (con::EMPTY_CELL == cellStr) {
					break;
				}

				// セルのデータを末尾に追加
				arrayStr.push_back(cellStr);
			}

			// csvファイルの読み取り位置をファイルの先頭に移動する
			m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

			return;
		}
	}

	// 一致する文字列がなかった場合
	arrayStr.push_back(m_NOT_FOUND);

	// csvファイルの読み取り位置をファイルの先頭に移動する
	m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);
}