#pragma once
#include "ghost_object.h"

class Player;
class Actor;
struct StAttackData;

/**
 * @brief �����蔻��̏���
 * �`�U������������I�`
*/
class Hitbox : public IGameObject
{
public:
    Hitbox();
    ~Hitbox();
    bool Start() override final;
    void Init(Player& otherPlayer, Actor& actor, StAttackData& attackData);
    void Update() override final;

    /**
     * @brief �U���������������̖��t���[���̏���
     * @return 
    */
    const bool UpdateCheckAttack();

    /**
     * @brief �����蔻��̏��̍X�V
    */
    void UpdateHitbox();


private:
    /**
     * @brief �����蔻����쐬
    */
    void Create();

    /**
     * @brief �U�����肪����v���C���[�̓����蔻��ɓ����������̊m�F
     * @return ����������
    */
    const bool CheckHit();

    /**
     * @brief �U�������������Ƃ��̏���
    */
    void HitAttack();



private:
    /**
     * @brief �U������G���A�̍폜
    */
    void Release()
    {
        if (false == m_flagInit) {
            return;
        }

        for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
            m_ghostBox[bodyPartsNum]->Release();
        }
    }


public: // get function
    /**
     * @brief �����蔻��̃S�[�X�g�I�u�W�F�N�g���擾
     * @param bodyParts �g�̂̕���
     * @return �w�肵���g�̂̕��ʂ̓����蔻��̃S�[�X�g�I�u�W�F�N�g
    */
    btGhostObject& GetGhostObject(const int bodyParts)
    {
        return *m_ghostBox[bodyParts]->GetGhostObject();
    }



private: // enum

    // ���f���͍��E���]���Ă���

    /**
     * @brief �g�̂̕���
    */
    const enum EnBodyParts
    {
        enHead,             // ��
        enBody,             // �g��
        enButt,             // �K
        // ��
        enLeftUpperArm,     // ��r
        enLeftForearm,      // �O�r
        enLeftHand,         // ��
        enLeftThighs,       // ������
        enLeftLowerLeg,     // ����
        enLeftLegs,         // ��
        // �E
        enRightUpperArm,    // ��r
        enRightForearm,     // �O�r
        enRightHand,        // ��
        enRightThighs,      // ������
        enRightLowerLeg,    // ����
        enRightLegs,        // ��
        enMaxBodyParts
    };


public:
    static const int m_MAX_EN_BODY_PARTS = EnBodyParts::enMaxBodyParts;




private: // data member
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    // �S�[�X�g�I�u�W�F�N�g�i�����蔻��Ɏg�p����{�b�N�X�j
    using GhostObjectPtr = std::unique_ptr<GhostObject>;
    std::vector<GhostObjectPtr> m_ghostBox;

    //////////////////////////////
    // ���̑�
    //////////////////////////////

    Player* m_getOtherPlayer = nullptr;
    Actor* m_getActor = nullptr;
    StAttackData* m_getStAttackData = nullptr;

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagInit = false; // ������������
};