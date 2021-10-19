#include "stdafx.h"
#include "attack_judgment.h"

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

    m_findPlayer = FindGO<Player>(igo::className::OTHER_PLAYER);

    return true;
}

void AttackJudgment::Update()
{

}

const bool AttackJudgment::CheckHit()
{
    bool checkHit = false; // “–‚½‚è”»’è‚ÆUŒ‚”»’è‚ªG‚ê‚Ä‚¢‚é‚©

    //ƒvƒŒƒCƒ„[‚Ì“–‚½‚è”»’è‚ÆUŒ‚”»’è‚ªG‚ê‚½‚©‚Ì”»’è‚¾‚ÆŽv‚¤
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