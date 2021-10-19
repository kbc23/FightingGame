#pragma once

class GhostObject;
class Player;

class AttackJudgment : public IGameObject
{
public:
    AttackJudgment();
    ~AttackJudgment();
    bool Start() override final;
    void Update() override final;


private:
    bool CheckHit();



public:
    /**
     * @brief �U������̔���G���A�̍쐬
     * @param pos �ʒu
     * @param rot ��]
    */
    void Create(const Vector3& pos, const Quaternion& rot, const Vector3& size);

    const float DebugRotation()
    {
        if (true == CheckHit()) {
            return 10.0f;
        }

        return 0.0f;
    }

private: // data menber
    GhostObject* m_ghostBox = nullptr;
    Player* m_findPlayer = nullptr;


};