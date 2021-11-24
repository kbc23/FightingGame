#pragma once
#include "ghost_object.h"

class Actor;

/**
 * @brief �����蔻��̏���
*/
class Hitbox : public IGameObject
{
public:
    Hitbox();
    ~Hitbox();
    bool Start() override final;
    void Init(Actor& actor);
    void Update() override final;


private:
    void Create();


    void UpdateHitbox();



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



private: // enum

    // ���f���͍��E���]���Ă���

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

    GhostObject* m_ghostBox[EnBodyParts::enMaxBodyParts] = { nullptr };

    //////////////////////////////
    // ���̑�
    //////////////////////////////

    Actor* m_getActor = nullptr;

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagInit = false; // ������������
};