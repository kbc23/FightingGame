#include "stdafx.h"
#include "read_CSV_file.h"

//////////////////////////////
// std::getline(A, B, C);
// A:�����𔲂��o������
// B:�����o�����������Ԃ����ޕ�
// C:�����o���ۂ̋�؂�ɂȂ���
//////////////////////////////



namespace con
{
	const int FIRST_OF_THE_ARRAY = 0; // �z��̍ŏ��̗v�f
	const int FIRST_OF_THE_READING_POSITION = 0; // �ǂݎ��ʒu�̍ŏ��̈ʒu
	const int NG_LESS_THAN_CELL_SPECIFICATION = 0; // �Z���̎w���NG�̒l�i�ȉ��j

	const std::string EMPTY_CELL = ""; // ��̃Z��
	const char CELL_DELIMITER = ','; // �Z���̋�؂�
}

std::string ReadCSVFile::m_NOT_FOUND = "not found"; // �f�[�^��������Ȃ������Ƃ��ɓ��͂��镶����

ReadCSVFile::ReadCSVFile()
{
	// TODO: �����悤�H
}

ReadCSVFile::ReadCSVFile(const std::string& openFile)
{
	// �t�@�C���p�X��ێ�
	m_filePath = openFile;
	// �t�@�C�����J��
	m_file.open(m_filePath);

	// �t�@�C�����J���Ȃ������Ƃ��̏���
	if (!m_file) {
		// TODO: �����Ƀt�@�C�����J���Ȃ������Ƃ��̏����������������ǁA
		// �����������炢�����S�R�v�����Ȃ��B
	}
}

ReadCSVFile::~ReadCSVFile()
{
	// �t�@�C�����J���Ă���ꍇ�A�t�@�C�������
	if (true == m_file.is_open()) {
		m_file.close();
	}
}

////////////////////////////////////////////////////////////
// �t�@�C���̊J�֘A
////////////////////////////////////////////////////////////

const bool ReadCSVFile::Open(const std::string& openFile)
{
	// ���łɊJ���Ă���
	if (true == IsOpen()) {
		return false;
	}

	// �t�@�C���p�X��ێ�
	m_filePath = openFile;
	// �t�@�C�����J��
	m_file.open(m_filePath);

	// �t�@�C�����J���Ȃ������Ƃ��̏���
	if (!m_file) {
		// TODO: �����Ƀt�@�C�����J���Ȃ������Ƃ��̏����������������ǁA
		// �����������炢�����S�R�v�����Ȃ��B
		return false;
	}

	return true;
}

void ReadCSVFile::Close()
{
	// �t�@�C�����J���Ă��Ȃ��ꍇ�A�������I��
	if (false == IsOpen()) {
		return;
	}

	// �t�@�C�������
	m_file.close();
	// �t�@�C���p�X��j��
	m_filePath.clear();
}

////////////////////////////////////////////////////////////
// �Z���̃f�[�^�̎擾�֘A
////////////////////////////////////////////////////////////

std::string ReadCSVFile::GetCell(const int xCell, const int yCell)
{
	if (false == IsOpen()) {
		return m_NOT_FOUND;
	}

	// �Z���̎w��łO�ȉ����w�肵�Ă���ꍇ
	if (con::NG_LESS_THAN_CELL_SPECIFICATION >= xCell ||
		con::NG_LESS_THAN_CELL_SPECIFICATION >= yCell) {
		return m_NOT_FOUND;
	}

	std::string getlineYStr; // Y���̍s��getline()�Ŏg�p����ϐ�
	std::string cellStr; // �擾�����Z���̕������ۊǂ���ϐ�

	// �w�肵���s������
	for (int checkYNum = con::FIRST_OF_THE_ARRAY; checkYNum < yCell; checkYNum++) {
		// �w�肵���s��������Ȃ��ꍇ
		if (true == m_file.eof()) {
			// csv�t�@�C���̓ǂݎ��ʒu���t�@�C���̐擪�Ɉړ�����
			m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

			return m_NOT_FOUND;
		}

		// �s�̃f�[�^���擾
		std::getline(m_file, getlineYStr);
	}

	std::stringstream YLineStr(getlineYStr); // �擾�����s�̃f�[�^��ۊǂ���ϐ�

	// �������s����w�肵���Z��������
	for (int checkCellNum = con::FIRST_OF_THE_ARRAY; checkCellNum < xCell; checkCellNum++) {
		// �w�肵���Z����������Ȃ��ꍇ
		if (true == YLineStr.eof()) {
			// csv�t�@�C���̓ǂݎ��ʒu���t�@�C���̐擪�Ɉړ�����
			m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

			return m_NOT_FOUND;
		}

		// �Z���̃f�[�^���擾
		std::getline(YLineStr, cellStr, con::CELL_DELIMITER);
	}

	// csv�t�@�C���̓ǂݎ��ʒu���t�@�C���̐擪�Ɉړ�����
	m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

	// �Z���̃f�[�^����̏ꍇ
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

	std::string getlineYStr; // Y���̍s��getline()�Ŏg�p����ϐ�
	std::string cellStr; // �擾�����Z���̕������ۊǂ���ϐ�
	bool flagWordMatch = false; // �w�肵��������Ɠ��������񂪂�������

	// �w�肵���L�[���[�h�ƈ�v����Z��������s������
	while (std::getline(m_file, getlineYStr)) {
		// �ŏ���csv�t�@�C���̈ꕶ�𔲂��o��
		// getline�Ŕ����o����悤�ɂ��邽�߂�stringstream�^�̕ϐ��ɓ����
		std::stringstream YLineStr(getlineYStr); // �擾�����s�̃f�[�^��ۊǂ���ϐ�

		// ��v���镶���񂪂��邩����
		while (std::getline(YLineStr, cellStr, con::CELL_DELIMITER)) {
			// ��v���镶���񂪂������ꍇ
			if (checkWord == cellStr) {
				flagWordMatch = true;

				break;
			}
		}

		// ��v���镶���񂪂������ꍇ
		if (true == flagWordMatch) {
			//std::stringstream r_str(getlineYStr);

			// ������̐擪�̈ʒu�ɃJ�[�\�����ړ�
			YLineStr.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

			// vector�z��Ɋi�[����
			while (std::getline(YLineStr, cellStr, con::CELL_DELIMITER)) {
				// �f�[�^����̃Z�����������ꍇ
				if (con::EMPTY_CELL == cellStr) {
					break;
				}

				// �Z���̃f�[�^�𖖔��ɒǉ�
				arrayStr.push_back(cellStr);
			}

			// csv�t�@�C���̓ǂݎ��ʒu���t�@�C���̐擪�Ɉړ�����
			m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);

			return;
		}
	}

	// ��v���镶���񂪂Ȃ������ꍇ
	arrayStr.push_back(m_NOT_FOUND);

	// csv�t�@�C���̓ǂݎ��ʒu���t�@�C���̐擪�Ɉړ�����
	m_file.seekg(con::FIRST_OF_THE_READING_POSITION, std::ios_base::beg);
}