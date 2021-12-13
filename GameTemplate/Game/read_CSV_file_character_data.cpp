#include "stdafx.h"

#include <iostream>
#include <vector>

#include "read_CSV_file_character_data.h"


void ReadCSVFileCharacterData::ReadCSVFileData(std::string filePath)
{
    // �t�@�C�����J��
    Open(filePath);
    
    // ���f���A�Z�b�g�̃t�H���_�[�̃p�X���擾
    std::string fileFolderPath = GetCell(2, 1);
    // �L�����N�^�[���f���̃t�@�C���l�[�����擾
    m_characterModelPath = GetCell(2, 2);
    // �A�j���[�V�����f�[�^�̃t�@�C���l�[�����擾
    GetLineByWord("�A�j���[�V����", m_characterAnimationPath);

    // �t�@�C�������
    Close();

    // �K�v�Ȃ������폜
    for (int animationNum = 0; m_characterAnimationPath.size() > animationNum; ++animationNum) {
        if ("�A�j���[�V����" == m_characterAnimationPath[animationNum]) {
            m_characterAnimationPath.erase(m_characterAnimationPath.begin() + animationNum);
            break;
        }
    }

    ////////////////////////////////////////////////////////////
    // �t�@�C���p�X���t�@�C���l�[���̑O�ɕt����
    ////////////////////////////////////////////////////////////

    // �t�@�C���p�X���t�@�C���l�[���̑O�ɕt����
    m_characterModelPath = fileFolderPath + m_characterModelPath;
    // �t�@�C���p�X���t�@�C���l�[���̑O�ɕt����
    for (int animationNum = 0; m_characterAnimationPath.size() > animationNum; ++animationNum) {
        m_characterAnimationPath[animationNum] = fileFolderPath + m_characterAnimationPath[animationNum];
    }
}