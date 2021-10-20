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
     * @brief 攻撃判定エリアの作成
     * @param pos 位置
     * @param rot 回転
    */
    void Create(const Vector3& pos, const Quaternion& rot, const Vector3& size);

    /**
     * @brief 攻撃判定エリアの削除
    */
    void Release()
    {
        m_ghostBox->Release();
    }

    /** Debug
     * @brief 当たり判定が攻撃判定に触れているとき、モデルを回転させるための数値を受け取る関数
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