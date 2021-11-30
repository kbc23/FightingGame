#include "stdafx.h"
#include "hitbox.h"

#include "player.h"
#include "actor.h"
#include "st_attack_data.h"
#include "st_defense_data.h"



namespace // constant
{
    // ���f���͍��E���]���Ă���

    // ���̖��O
    constexpr const wchar_t* BONE_NAME[Hitbox::m_MAX_BODY_PARTS] =
    {
        L"J_Bip_C_Head",        // ��
        L"J_Bip_C_UpperChest",  // �g��
        L"J_Bip_C_Spine",       // �K
        // ��
        L"J_Bip_L_UpperArm",    // ��r
        L"J_Bip_L_LowerArm",    // �O�r
        L"J_Bip_L_Hand",        // ��
        L"J_Bip_L_UpperLeg",    // ������
        L"J_Bip_L_LowerLeg",    // ����
        L"J_Bip_L_Foot",        // ��
        // �E
        L"J_Bip_R_UpperArm",    // ��r
        L"J_Bip_R_LowerArm",    // �O�r
        L"J_Bip_R_Hand",        // ��
        L"J_Bip_R_UpperLeg",    // ������
        L"J_Bip_R_LowerLeg",    // ����
        L"J_Bip_R_Foot"         // ��
    };

    // �����蔻��̃T�C�Y
    const Vector3 HITBOX_SIZE[Hitbox::m_MAX_BODY_PARTS] =
    {
        { 20.0f,20.0f,20.0f },  // ��
        { 25.0f,30.0f,20.0f },  // �g��
        { 25.0f,10.0f,20.0f },  // �K
        // ��
        { 20.0f,6.0f,6.0f },    // ��r
        { 17.0f,6.0f,6.0f },    // �O�r
        { 8.0f,8.0f,8.0f },     // ��
        { 12.0f,30.0f,12.0f },  // ������
        { 12.0f,30.0f,12.0f },  // ����
        { 8.0f,8.0f,18.0f },    // ��
        // �E
        { 20.0f,6.0f,6.0f },    // ��r
        { 17.0f,6.0f,6.0f },    // �O�r
        { 8.0f,8.0f,8.0f },     // ��
        { 12.0f,30.0f,12.0f },  // ������
        { 12.0f,30.0f,12.0f },  // ����
        { 8.0f,8.0f,18.0f }     // ��
    };

    // �����蔻��̈ʒu����
    const Vector3 HITBOX_POSITION_ADJUSTMENT[Hitbox::m_MAX_BODY_PARTS] =
    {
        { 0.0f,8.0f,0.0f },     // ��
        { 0.0f,0.0f,0.0f },     // �g��
        { 0.0f,-5.0f,0.0f },    // �K
        // ��
        { 10.0f,0.0f,0.0f },    // ��r
        { 12.0f,0.0f,0.0f },    // �O�r
        { 5.0f,0.0f,0.0f },     // ��
        { 0.0f,-15.0f,0.0f },   // ������
        { 0.0f,-15.0f,-2.0f },   // ����
        { 0.0f,0.0f,5.0f },     // ��
        // �E
        { -10.0f,0.0f,0.0f },   // ��r
        { -12.0f,0.0f,0.0f },   // �O�r
        { -5.0f,0.0f,0.0f },    // ��
        { 0.0f,-15.0f,0.0f },   // ������
        { 0.0f,-15.0f,-2.0f },   // ����
        { 0.0f,0.0f,5.0f }      // ��
    };
}



Hitbox::Hitbox()
{
    // �g�̂̃p�[�c���ׂĂ�����z��̃T�C�Y���m��
    m_ghostBox.resize(EnBodyParts::enMaxBodyParts);

    // �|�C���^���쐬
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        m_ghostBox[bodyPartsNum].reset(new GhostObject);
    }
}

Hitbox::~Hitbox()
{
    // �|�C���^���폜
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        m_ghostBox[bodyPartsNum].reset();
    }
}

bool Hitbox::Start()
{
    return true;
}

void Hitbox::Init(Player& otherPlayer, Actor& actor, StAttackData& attackData, StDefenseData& defenseData)
{
    // �����Ŏ����Ă����������̃N���X�ŕێ�
    m_getOtherPlayer = &otherPlayer;
    m_getActor = &actor;
    m_getStAttackData = &attackData;
    m_getStDefenseData = &defenseData;

    // �L�����N�^�[��ModelRender�N���X�ňʒu���̍X�V�����������߁A
    // ������ModelRender�N���X�Ɏ��g�̏���n��
    // �i�֐����ς��邱�Ɓj
    m_getActor->SetTest(*this);

    // �{�b�N�X�̍쐬
    //Create();

    m_flagInit = true;
}

void Hitbox::Update()
{

}

void Hitbox::Create()
{
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        // ���̖��O�Ń{�[��ID������
        int boneId = m_getActor->GetSkeleton().FindBoneID(BONE_NAME[bodyPartsNum]);
        // ���������{�[��ID���g�p���āA�{�[�����擾
        Bone* bone = m_getActor->GetSkeleton().GetBone(boneId);
        // �{�[���̃��[���h�s����擾
        Matrix boneMatrix = bone->GetWorldMatrix();

        // �s�񂩂�ʒu�����擾
        Vector3 boxPos = boneMatrix.GetTranslation();
        
        // �s�񂩂��]�����擾
        Quaternion boxRot = Quaternion::Identity;
        boxRot.SetRotation(boneMatrix);

        // �ʒu�̒���
        Vector3 boxPositionAdjustment = HITBOX_POSITION_ADJUSTMENT[bodyPartsNum];
        boxRot.Apply(boxPositionAdjustment);
        boxPos += boxPositionAdjustment;

        // �����蔻����쐬
        m_ghostBox[bodyPartsNum]->CreateBox(boxPos, boxRot, HITBOX_SIZE[bodyPartsNum]);
    }
}

void Hitbox::UpdateHitbox()
{
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        // ���̖��O�Ń{�[��ID������
        int boneId = m_getActor->GetSkeleton().FindBoneID(BONE_NAME[bodyPartsNum]);
        // ���������{�[��ID���g�p���āA�{�[�����擾
        Bone* bone = m_getActor->GetSkeleton().GetBone(boneId);
        // �{�[���̃��[���h�s����擾
        Matrix boneMatrix = bone->GetWorldMatrix();

        // �s�񂩂�ʒu�����擾
        Vector3 boxPos = boneMatrix.GetTranslation();

        // �s�񂩂��]�����擾
        Quaternion boxRot = Quaternion::Identity;
        boxRot.SetRotation(boneMatrix);

        // �ʒu�̒���
        Vector3 boxPositionAdjustment = HITBOX_POSITION_ADJUSTMENT[bodyPartsNum];
        boxRot.Apply(boxPositionAdjustment);
        boxPos += boxPositionAdjustment;

        // �����蔻��̏����X�V
        m_ghostBox[bodyPartsNum]->UpdateGhostObject(boxPos, boxRot);
    }
}

const bool Hitbox::UpdateCheckAttack()
{
    // �U�����ł͂Ȃ�
    if (false == m_getStAttackData->GetFlagAttackNow()) {
        return false;
    }
    
    // ���łɍU�����������Ă���
    if (true == m_getStAttackData->GetFlagAlreadyAttacked()) {
        return false;
    }

    // �U��������������
    int hitBodyParts = CheckHit();
    if (EnBodyParts::enMaxBodyParts != hitBodyParts) {
        // �U�������������Ƃ��̏���
        HitAttack();

        // �����Ŗh�䂵�Ă邩�A���������ꏊ����A�O�r���̊m�F�������Ȃ�
        if (true == m_getOtherPlayer->GetStDefenseData().GetFlagDefense() &&
            true == CheckHitDefenseBodyParts(hitBodyParts)) {
            // �U���͓����������ǁA�_���[�W�͂Ȃ�
            return false;
        }

        // �U������������
        return true;
    }

    // �U���͓������ĂȂ�
    return false;
}

const int Hitbox::CheckHit()
{
    int checkHitBodyParts = EnBodyParts::enMaxBodyParts; // �����蔻��ƍU�����肪�G��Ă��邩

    //�v���C���[�̓����蔻��ƍU�����肪�G�ꂽ���̔���
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        PhysicsWorld::GetInstance()->ContactTest(&m_getOtherPlayer->GetGhostObject(bodyPartsNum),
            [&](const btCollisionObject& contactObject)
            {
                // ���肪���������Ƃ�
                if (m_ghostBox[EnBodyParts::enLeftHand]->IsSelf(contactObject)) {
                    checkHitBodyParts = bodyPartsNum;
                    return;
                }
            });

        // ���������画�菈�����甲����
        if (EnBodyParts::enMaxBodyParts != checkHitBodyParts) {
            break;
        }
    }

    return checkHitBodyParts;
}

const bool Hitbox::CheckHitDefenseBodyParts(const int bodyParts)
{
    // ��ɓ���������
    if (EnBodyParts::enLeftHand == bodyParts || EnBodyParts::enRightHand == bodyParts) {
        return true;
    }
    // �O�r�ɓ���������
    if (EnBodyParts::enLeftForearm == bodyParts || EnBodyParts::enRightForearm == bodyParts) {
        return true;
    }

    // �������Ă��Ȃ��ꍇ
    return false;
}

void Hitbox::HitAttack()
{
    // ���łɍU���������������Ƃ�`����
    m_getStAttackData->SetFlagAlreadyAttacked(true);
}