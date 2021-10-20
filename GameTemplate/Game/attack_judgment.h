#pragma once
#include "ghost_object.h"

class Player;

class AttackJudgment : public IGameObject
{
public:
    AttackJudgment();
    ~AttackJudgment();
    bool Start() override final;
    void Update() override final;


public:
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
        m_ghostBox->Release();
    }

    /** Debug
     * @brief �����蔻�肪�U������ɐG��Ă���Ƃ��A���f������]�����邽�߂̐��l���󂯎��֐�
     * @return 
    */
    const float DebugRotation()
    {
        if (true == CheckHit()) {
            return 1.0f;
        }

        return 0.0f;
    }

private: // data menber
    GhostObject* m_ghostBox = nullptr;
    Player* m_findPlayer = nullptr;


};