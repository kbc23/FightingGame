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
    // �����蔻��̃L�������v���C���[���Ƃɕς��Ȃ��Ƃ����Ȃ��̂�
    // �����Ńv���C���[�ԍ��𗘗p����FindGO����v���C���[��I������
    if (playerNum == m_findGameData->GetPlayerNum()) {
        m_findPlayer = FindGO<Player>(igo::className::OTHER_PLAYER);
    }
    else if (playerNum == m_findGameData->GetOtherPlayerNum()) {
        m_findPlayer = FindGO<Player>(igo::className::PLAYER);
    }
    else {
        // ���������s
        return;
    }

    m_flagInit = true;
}

void AttackJudgment::Update()
{

}

const bool AttackJudgment::CheckHit()
{
    bool checkHit = false; // �����蔻��ƍU�����肪�G��Ă��邩

    //�v���C���[�̓����蔻��ƍU�����肪�G�ꂽ���̔��肾�Ǝv��
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