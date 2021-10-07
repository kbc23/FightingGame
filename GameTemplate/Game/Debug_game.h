#pragma once

class DebugGame
{
public:
	/// <summary>
	/// �C���X�^���X�̍쐬�B
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new DebugGame;
	}
	/// <summary>
	/// �C���X�^���X�̔j���B
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}
	/// <summary>
	/// �C���X�^���X���擾�B
	/// </summary>
	/// <returns></returns>
	static DebugGame* GetInstance()
	{
		return m_instance;
	}


public:
	/**
	 * @brief �f�o�b�O���[�h��
	 * @return �f�o�b�O���[�h���ǂ���
	*/
	const bool DebugMode()
	{
		return m_debugMode;
	}
    /**
     * @brief �f�o�b�O�̃\�����[�h��
     * @return �f�o�b�O�̃\�����[�h���ǂ���
    */
    const bool DebugSoloMode()
    {
		if (false == m_debugMode) {
			return false;
		}

        return m_debugSoloMode;
    }



public:
	/**
	 * @brief �f�o�b�O�ɐݒ�
	*/
	void OnDebugSoloMode()
	{
		m_debugMode = true;
	}
	/**
	 * @brief �f�o�b�O�łȂ�����
	*/
	void OffDebugSoloMode()
	{
		m_debugMode = false;
	}
	/**
	 * @brief �f�o�b�O�̃\�����[�h�ɐݒ�
	*/
	void OnDebugSoloMode()
	{
		if (false == m_debugMode) {
			return;
		}

		m_debugSoloMode = true;
	}
	/**
	 * @brief �f�o�b�O�̃\�����[�h�łȂ�����
	*/
	void OffDebugSoloMode()
	{
		if (false == m_debugMode) {
			return;
		}

		m_debugSoloMode = false;
	}



private: // data menber

	static DebugGame* m_instance;		//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ�

	////////////////////////////////////////////////////////////
	// �t���O
	////////////////////////////////////////////////////////////

	bool m_debugMode = false; // �f�o�b�O���[�h��
    bool m_debugSoloMode = false; // �f�o�b�O�̃\�����[�h��





};