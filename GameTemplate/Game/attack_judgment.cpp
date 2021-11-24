#include "stdafx.h"
#include "attack_judgment.h"

#include "player.h"
#include "game_data.h"

AttackJudgment::AttackJudgment()
{
    m_ghostBox = new GhostObject;

    m_findGameData = FindGO<GameData>(igo::className::GAME_DATA);
}

AttackJudgment::~AttackJudgment()
{
    delete m_ghostBox;
}

bool AttackJudgment::Start()
{
    return true;
}

void AttackJudgment::Init(const int playerNum)
{
    // 当たり判定のキャラをプレイヤーごとに変えないといけないので
    // ここでプレイヤー番号を利用してFindGOするプレイヤーを選択する
    if (playerNum == m_findGameData->GetPlayerNum()) {
        m_findPlayer = FindGO<Player>(igo::className::OTHER_PLAYER);
    }
    else if (playerNum == m_findGameData->GetOtherPlayerNum()) {
        m_findPlayer = FindGO<Player>(igo::className::PLAYER);
    }
    else {
        // 初期化失敗
        return;
    }

    m_flagInit = true;
}

void AttackJudgment::Update()
{

}

const bool AttackJudgment::CheckHit()
{
    bool checkHit = false; // 当たり判定と攻撃判定が触れているか

    //プレイヤーの当たり判定と攻撃判定が触れたかの判定だと思う
    //PhysicsWorld::GetInstance()->ContactTest(*m_findPlayer->GetRigidBody(),
    //    [&](const btCollisionObject& contactObject)
    //    {
    //        if (m_ghostBox->IsSelf(contactObject)) {
    //            checkHit = true;
    //        }
    //    });

    return checkHit;
}

void AttackJudgment::Create(const Vector3& pos, const Quaternion& rot, const Vector3& size)
{
    if (false == m_flagInit) {
        return;
    }

    m_ghostBox->CreateBox(pos, rot, size);
}