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
	// �t�@�C���̊J�֘A
	////////////////////////////////////////////////////////////
	/**
	 * @brief �t�@�C�����J��
	 * @param open_file �J���t�@�C���̖��O
	 * @return �J������
	*/
	const bool Open(const std::string& openFile);
	/**
	 * @brief �t�@�C�������
	*/
	void Close();
	/**
	 * @brief �t�@�C�����J���Ă��邩���m�F����
	 * @return �J���Ă��邩
	*/
	const bool IsOpen()
	{
		return m_file.is_open();
	}
	/**
	 * @brief �ێ����Ă���t�@�C���p�X���擾
	 * @return �ێ����Ă���t�@�C���p�X
	*/
	const std::string GetOpenFilePath() const
	{
		// �t�@�C���p�X��ێ����Ă��Ȃ��ꍇ
		if (true == m_filePath.empty()) {
			return "No file path is retained";
		}

		return m_filePath;
	}

	////////////////////////////////////////////////////////////
	// �Z���̃f�[�^�̎擾�֘A
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// �w�肵���Z���̃f�[�^���擾
	//////////////////////////////
	/**
	 * @brief �w�肵���Z���̃f�[�^���擾����
	 * @param x �Z����X���W
	 * @param y �Z����Y���W
	 * @return �擾�����Z���̃f�[�^
	*/
	std::string GetCell(const int xCell, const int yCell);

	//////////////////////////////
	// �w�肵���L�[���[�h������s�̂��ׂẴZ���̃f�[�^���擾
	//////////////////////////////

	/**
	 * @brief ��L�̃I�[�o�[���C�h(vector<std::string>)
	 * @param check_word �L�[���[�h
	 * @param array_str �擾�����Z���̃f�[�^������z��
	*/
	void GetLineByWord(const std::string& checkWord, std::vector<std::string>& arrayStr);


protected: //constant
	static std::string m_NOT_FOUND; // �f�[�^��������Ȃ������Ƃ��ɓ��͂��镶����


private: // data member
    std::ifstream m_file; // �J�����t�@�C���f�[�^��ێ�����ϐ�
	std::string m_filePath; // �J���Ă���t�@�C���̃t�@�C���p�X
};