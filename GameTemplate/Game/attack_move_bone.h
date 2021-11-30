////////////////////////////////////////////////////////////
// �g�p���Ȃ�
////////////////////////////////////////////////////////////
#pragma once

class Player;
class Actor;
struct StAttackData;

////////////////////////////////////////////////////////////
// 1.�U�������̊m�F
// 2.�U���^�[�Q�b�g���ʂ̊m�F
// 3.�U���^�[�Q�b�g�Ɍ����Ď�̃{�[���̈ʒu�𒲐����鏈��
// 3.1.�Ƃ肠�����A�������̒����͂����ɁA�c���������̒���������B
////////////////////////////////////////////////////////////

/**
 * @brief �U�����[�V�������̃{�[���̈ړ�����
*/
class AttackMoveBone : public IGameObject
{
public:
    AttackMoveBone();
    ~AttackMoveBone();
    bool Start() override final;
    void Update() override final;


    void Init(Player& otherPlayer, Actor& actor, StAttackData& stAttackData);


private:
    const int CheckTargetBodyParts();


    void MoveBoneToAttackTheHead();

    void MoveBoneToAttackTheBody();



private: // enum
    /**
     * @brief �ǂ̕��ʂ�_���čU�����Ă��邩
    */
    enum EnTargetBodyParts
    {
        enHead,                 // ��
        enBody,                 // ����
        enMaxTargetBodyParts
    };



private: // data member
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////



    //////////////////////////////
    // ���̃N���X���玝���Ă����C���X�^���X
    //////////////////////////////

    Player* m_getOhterPlayer = nullptr;
    Actor* m_getActor = nullptr;
    StAttackData* m_getStAttackData = nullptr;
};