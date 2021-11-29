#pragma once
#include  "constant.h"
#include "ghost_object.h"

class Player;
class Actor;
struct StAttackData;
struct StDefenseData;

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
    void Update() override final;

    /**
     * @brief ������
     * @param otherPlayer ����v���C���[�̃C���X�^���X
     * @param actor Actor�N���X�̃C���X�^���X
     * @param attackData �U�����̃C���X�^���X
     * @param defenseData �h����̃C���X�^���X
    */
    void Init(Player& otherPlayer, Actor& actor, StAttackData& attackData, StDefenseData& defenseData);

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
    const int CheckHit();

    /**
     * @brief �h�䎞�Ɏ肪��r�ɓ������������m�F
     * @param bodyParts ������������
     * @return �肩��r�ɓ������Ă�����
    */
    const bool CheckHitDefenseBodyParts(const int bodyParts);

    /**
     * @brief �U�������������Ƃ��̏���
    */
    void HitAttack();


public: // get function
    /**
     * @brief �����蔻��̃S�[�X�g�I�u�W�F�N�g���擾
     * @param bodyParts �g�̂̕���
     * @return �w�肵���g�̂̕��ʂ̓����蔻��̃S�[�X�g�I�u�W�F�N�g
    */
    btGhostObject& GetGhostObject(const int bodyParts) const
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
        // �ő�l
        enMaxBodyParts
    };


public: // constant
    static const int m_MAX_BODY_PARTS = EnBodyParts::enMaxBodyParts; // �g�̂̕��ʂ̍ő�l�i�z��̗v�f���j


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
    // ���̃N���X���玝���Ă����C���X�^���X
    //////////////////////////////

    Player* m_getOtherPlayer = nullptr; // ����v���C���[
    Actor* m_getActor = nullptr; // �L�����N�^�[�̃��f���֘A
    StAttackData* m_getStAttackData = nullptr; // �U�����
    StDefenseData* m_getStDefenseData = nullptr; // �h����

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagInit = false; // ������������
};