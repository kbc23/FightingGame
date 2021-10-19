#include "stdafx.h"
#include "attack_judgment.h"

#include "ghost_object.h"
#include "player.h"

AttackJudgment::AttackJudgment()
{

}

AttackJudgment::~AttackJudgment()
{
    delete m_ghostBox;
}

bool AttackJudgment::Start()
{
    m_ghostBox = new GhostObject;

    m_findPlayer = FindGO<Player>(igo::className::PLAYER);

    return true;
}

void AttackJudgment::Update()
{

}

bool AttackJudgment::CheckHit()
{
    bool checkHit = false;

    //プレイヤーの当たり判定と攻撃判定が触れたかの判定だと思う
    PhysicsWorld::GetInstance()->ContactTest(*m_findPlayer->GetRigidBody(),
        [&](const btCollisionObject& contactObject)
        {
            if (m_ghostBox->IsSelf(contactObject)) {
                checkHit = true;
            }
        });

    return checkHit;
}

void AttackJudgment::Create(const Vector3& pos, const Quaternion& rot, const Vector3& size)
{
    m_ghostBox->CreateBox(pos, rot, size);
}