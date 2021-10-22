#pragma once
#include "ghost_object.h"

class Player;
class GameData;

class AttackJudgment : public IGameObject
{
public:
    AttackJudgment();
    ~AttackJudgment();
    bool Start() override final;
    void Update() override final;

    /**
     * @brief �������i�Ă΂Ȃ��Ɠ������Ȃ��̂ŕK���ĂԂ��Ɓj
     * @param playerNum �v���C���[�ԍ�
    */
    void Init(const int playerNum);


public:
    /**
     * @brief �U�����肪����v���C���[�̓����蔻��ɓ����������̊m�F
     * @return ����������
    */
    const bool CheckHit();



public:
    /**
     * @brief �U������G���A�̍쐬
     * @param pos �ʒu
     * @param rot ��]
    */
    void Create(const Vector3& pos, const Quaternion& rot, const Vector3& size);

    /**
     * @brief �U������G���A�̍폜
    */
    void Release()
    {
        if (false == m_flagInit) {
            return;
        }

        m_ghostBox->Release();
    }


private: // data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    GhostObject* m_ghostBox = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////

    Player* m_findPlayer = nullptr;
    GameData* m_findGameData = nullptr;

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagInit = false; // ������������
};