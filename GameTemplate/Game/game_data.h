#pragma once

/**
 * @brief �Q�[���̂��낢��ȂƂ���Ŏg�p����f�[�^��ۑ�����N���X
 * �����ӁI
 * �d�l��A���낢��ȂƂ���ŕۑ����Ȃ���΂Ȃ�Ȃ����̂�������邱�ƁI
*/
class GameData : public IGameObject
{
public:
    GameData();
    ~GameData();
    bool Start() override final;
    void Update() override final;



public: // Get function
    /**
     * @brief �v���C���[�̔ԍ����擾
     * @return �v���C���[�̔ԍ�
    */
    const int GetPlayerNum()
    {
        return m_playerNum;
    }

    /**
     * @brief ����v���C���[�̔ԍ����擾
     * @return ����v���C���[�̔ԍ�
    */
    const int GetOtherPlayerNum()
    {
        return m_otherPlayerNum;
    }


public: // Set function
    /**
     * @brief �v���C���[�̔ԍ��Ƒ���v���C���[�̔ԍ���ۑ�
     * @param playerNum �v���C���[�̔ԍ�
     * @param otherPlayerNum ����v���C���[�̔ԍ�
    */
    void SetPlayerNumAndOtherPlayerNum(const int playerNum, const int otherPlayerNum)
    {
        m_playerNum = playerNum;
        m_otherPlayerNum = otherPlayerNum;
    }


private: // data member
    int m_playerNum = 0; // �v���C���[�̔ԍ�
    int m_otherPlayerNum = 0; // ����v���C���[�̔ԍ�


};