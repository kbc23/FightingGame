////////////////////////////////////////////////////////////
// �g�p���Ȃ�
////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "attack_move_bone.h"

#include "player.h"
#include "actor.h"
#include "st_attack_data.h"



namespace // constant
{
    constexpr const wchar_t* HEAD_BONE_NAME = L"J_Bip_C_Head"; // ���̍��̖��O
    constexpr const wchar_t* L_HAND_BONE_NAME = L"J_Bip_L_UpperArm"; // ����̍��̖��O
    constexpr const wchar_t* R_HAND_BONE_NAME = L"J_Bip_R_Hand"; // �E��̍��̖��O
}



AttackMoveBone::AttackMoveBone()
{

}

AttackMoveBone::~AttackMoveBone()
{

}

bool AttackMoveBone::Start()
{
    return true;
}

void AttackMoveBone::Init(Player& otherPlayer, Actor& actor, StAttackData& stAttackData)
{
    m_getOhterPlayer = &otherPlayer;
    m_getActor = &actor;
    m_getStAttackData = &stAttackData;
}

void AttackMoveBone::Update()
{
    // ���ݍU���������m�F
    // �U�����łȂ���΁A���������Ȃ�
    if (false == m_getStAttackData->GetFlagAttackNow())
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    // �U�����̍��̏���
    ////////////////////////////////////////////////////////////

    int targetBodyParts = CheckTargetBodyParts();

    if (EnTargetBodyParts::enHead == targetBodyParts) {
        MoveBoneToAttackTheHead();
    }
    else if (EnTargetBodyParts::enBody == targetBodyParts) {
        MoveBoneToAttackTheBody();
    }
}

const int AttackMoveBone::CheckTargetBodyParts()
{
    // �ǂ��ɂ����āA�ǂ̕��ʂ�_���Ă邩�̏�����ɓ����

    return EnTargetBodyParts::enHead;
}

void AttackMoveBone::MoveBoneToAttackTheHead()
{
    // ���݁A�e�X�g�i�K�ł͍���i���f���͔��]���ĕ\������Ă���j�ōU�����Ă���̂ŁA
    // ����̍��̓����𒲐�����B

    Vector3 boneMove = Vector3::Zero; // ��������{�[���̈ړ���

    ////////////////////////////////////////////////////////////
    // �{�[���̈ʒu�����擾
    ////////////////////////////////////////////////////////////

    // ���̖��O�Ń{�[��ID������
    int handBoneId = m_getActor->GetSkeleton().FindBoneID(L_HAND_BONE_NAME);
    // ���������{�[��ID���g�p���āA�{�[�����擾
    Bone* handBone = m_getActor->GetSkeleton().GetBone(handBoneId);
    // �{�[���̃��[���h�s����擾
    Matrix handBoneMatrix = handBone->GetWorldMatrix();
    // �s�񂩂�ʒu�����擾
    Vector3 handBonePosition = handBoneMatrix.GetTranslation();

    ////////////////////////////////////////////////////////////
    // �^�[�Q�b�g�ɂȂ�{�[���̈ʒu���擾
    // �������Ń^�[�Q�b�g�̃{�[�����������āA�֐����P�ɂ��Ă�������
    ////////////////////////////////////////////////////////////

    // ���̖��O�Ń{�[��ID������
    int targetBoneId = m_getOhterPlayer->GetActor().GetSkeleton().FindBoneID(HEAD_BONE_NAME);
    // ���������{�[��ID���g�p���āA�{�[�����擾
    Bone* targetBone = m_getOhterPlayer->GetActor().GetSkeleton().GetBone(targetBoneId);
    // �{�[���̃��[���h�s����擾
    Matrix targetBoneMatrix = targetBone->GetWorldMatrix();
    // �s�񂩂�ʒu�����擾
    Vector3 targetBonePosition = targetBoneMatrix.GetTranslation();

    ////////////////////////////////////////////////////////////
    // �{�[���̈ʒu�̒����������Ȃ�
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // ��̃{�[���ƃ^�[�Q�b�g�̃{�[���̈ʒu�̍ŒZ�������擾���āA
    // ���݂̎�̃{�[����X�AZ���W���̈ʒu�̍ŒZ������Y���W����̃{�[����Y���W�ɃZ�b�g����
    //////////////////////////////

    // ���ʒu�Ōv�Z���悤�Ǝv�������A
    // ��ƃ^�[�Q�b�g�̈ʒu�����g�p���āA��r�̃{�[������]�����ă^�[�Q�b�g�Ɍ����čU������B

    Quaternion rotation = g_quatIdentity;

    rotation.SetRotation(Vector3::Front, targetBonePosition);

    Matrix rotMatrix;
    rotMatrix.MakeRotationFromQuaternion(rotation);

    // ���̖��O�Ń{�[��ID������
    int localhandBoneId = m_getActor->GetSkeleton().FindBoneID(L_HAND_BONE_NAME);
    // ���������{�[��ID���g�p���āA�{�[�����擾
    Bone* localhandBone = m_getActor->GetSkeleton().GetBone(localhandBoneId);
    // �{�[���̃��[���h�s����擾
    Matrix localhandBoneMatrix = localhandBone->GetLocalMatrix();
    // �s�񂩂�ʒu�����擾
    Vector3 localhandBonePosition = localhandBoneMatrix.GetTranslation();

    localhandBoneMatrix *= rotMatrix;






    //float handVectorLength = handBonePosition.Length();
    //float targetVectorLength = targetBonePosition.Length();

    //float nowPositionLength = handVectorLength / targetVectorLength;

    //boneMove.y = targetBonePosition.y * nowPositionLength;

    //handBonePosition.y = targetBonePosition.y * nowPositionLength;



    






    //////////////////////////////
    // �ŏI�����A�Z�b�g
    //////////////////////////////

    // �Ƃ肠����Y���W�̒��������������̂�X�AZ���W���O�ɂ���
    boneMove.x = 0.0f;
    boneMove.z = 0.0f;

    //handBonePosition.Normalize();

    //handBonePosition += boneMove;

    // ���������ʒu���Z�b�g
    //handBoneMatrix.SetTranslation(handBonePosition);

    // ���[���h�s�� = ���[�J���s�� * ���f���̃��[���h�s��
    // ���[�J���s�� = ���[���h�s�� / ���f���̃��[���h�s��
    // �s��ɂ́A����Z���Ȃ��Ƃ̂���
    //Matrix local = handBoneMatrix / m_getActor->GetModelWorldMatrix();

    handBone->SetLocalMatrix(localhandBoneMatrix);

}

void AttackMoveBone::MoveBoneToAttackTheBody()
{

}