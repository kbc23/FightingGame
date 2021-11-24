#include "stdafx.h"
#include "hitbox.h"

#include "actor.h"



namespace
{
    // ���f���͍��E���]���Ă���

    constexpr const wchar_t* BONE_NAME[Hitbox::m_MAX_EN_BODY_PARTS] =
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

    const Vector3 BONE_SIZE[Hitbox::m_MAX_EN_BODY_PARTS] =
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

    const Vector3 POSITION_ADJUSTMENT[Hitbox::m_MAX_EN_BODY_PARTS] =
    {
        { 0.0f,8.0f,0.0f },     // ��
        { 0.0f,0.0f,0.0f },     // �g��
        { 0.0f,-5.0f,0.0f },    // �K
        // ��
        { 10.0f,0.0f,0.0f },    // ��r
        { 12.0f,0.0f,0.0f },    // �O�r
        { 5.0f,0.0f,0.0f },     // ��
        { 0.0f,-15.0f,0.0f },   // ������
        { 0.0f,-15.0f,0.0f },   // ����
        { 0.0f,0.0f,5.0f },     // ��
        // �E
        { -10.0f,0.0f,0.0f },   // ��r
        { -12.0f,0.0f,0.0f },   // �O�r
        { -5.0f,0.0f,0.0f },    // ��
        { 0.0f,-15.0f,0.0f },   // ������
        { 0.0f,-15.0f,0.0f },   // ����
        { 0.0f,0.0f,5.0f }      // ��
    };
}



Hitbox::Hitbox()
{
    m_ghostBox.resize(EnBodyParts::enMaxBodyParts);

    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        m_ghostBox[bodyPartsNum].reset(new GhostObject);
    }
}

Hitbox::~Hitbox()
{
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        m_ghostBox[bodyPartsNum].reset();
    }
}

bool Hitbox::Start()
{
    return true;
}

void Hitbox::Init(Actor& actor)
{
    m_getActor = &actor;

    Create();

    m_flagInit = true;
}

void Hitbox::Update()
{
    UpdateHitbox();
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

        // �ʒu���X�V
        Vector3 boxPositionAdjustment = POSITION_ADJUSTMENT[bodyPartsNum];
        boxRot.Apply(boxPositionAdjustment);
        boxPos += boxPositionAdjustment;

        // �����蔻����쐬
        m_ghostBox[bodyPartsNum]->CreateBox(boxPos, boxRot, BONE_SIZE[bodyPartsNum]);

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

        // �ʒu���X�V
        Vector3 boxPositionAdjustment = POSITION_ADJUSTMENT[bodyPartsNum];
        boxRot.Apply(boxPositionAdjustment);
        boxPos += boxPositionAdjustment;

        // �����蔻��̏����X�V
        m_ghostBox[bodyPartsNum]->UpdateGhostObject(boxPos, boxRot);
    }
}