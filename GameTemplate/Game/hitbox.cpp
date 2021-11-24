#include "stdafx.h"
#include "hitbox.h"

#include "actor.h"



namespace
{
    // モデルは左右反転している

    constexpr const wchar_t* BONE_NAME[Hitbox::m_MAX_EN_BODY_PARTS] =
    {
        L"J_Bip_C_Head",        // 頭
        L"J_Bip_C_UpperChest",  // 身体
        L"J_Bip_C_Spine",       // 尻
        // 左
        L"J_Bip_L_UpperArm",    // 上腕
        L"J_Bip_L_LowerArm",    // 前腕
        L"J_Bip_L_Hand",        // 手
        L"J_Bip_L_UpperLeg",    // 太もも
        L"J_Bip_L_LowerLeg",    // 下腿
        L"J_Bip_L_Foot",        // 足
        // 右
        L"J_Bip_R_UpperArm",    // 上腕
        L"J_Bip_R_LowerArm",    // 前腕
        L"J_Bip_R_Hand",        // 手
        L"J_Bip_R_UpperLeg",    // 太もも
        L"J_Bip_R_LowerLeg",    // 下腿
        L"J_Bip_R_Foot"         // 足
    };

    const Vector3 BONE_SIZE[Hitbox::m_MAX_EN_BODY_PARTS] =
    {
        { 20.0f,20.0f,20.0f },
        { 25.0f,30.0f,20.0f },
        { 25.0f,10.0f,20.0f },
        // 左
        { 20.0f,6.0f,6.0f },
        { 17.0f,6.0f,6.0f },
        { 8.0f,8.0f,8.0f },
        { 12.0f,30.0f,12.0f },
        { 12.0f,30.0f,12.0f },
        { 8.0f,8.0f,18.0f },
        // 右
        { 20.0f,6.0f,6.0f },
        { 17.0f,6.0f,6.0f },
        { 8.0f,8.0f,8.0f },
        { 12.0f,30.0f,12.0f },
        { 12.0f,30.0f,12.0f },
        { 8.0f,8.0f,18.0f }
    };

    const Vector3 POSITION_ADJUSTMENT[Hitbox::m_MAX_EN_BODY_PARTS] =
    {
        { 0.0f,8.0f,0.0f },
        { 0.0f,0.0f,0.0f },
        { 0.0f,-5.0f,0.0f },
        // 左
        { 10.0f,0.0f,0.0f },
        { 12.0f,0.0f,0.0f },
        { 5.0f,0.0f,0.0f },
        { 0.0f,-15.0f,0.0f },
        { 0.0f,-15.0f,0.0f },
        { 0.0f,0.0f,5.0f },
        // 右
        { -10.0f,0.0f,0.0f },
        { -12.0f,0.0f,0.0f },
        { -5.0f,0.0f,0.0f },
        { 0.0f,-15.0f,0.0f },
        { 0.0f,-15.0f,0.0f },
        { 0.0f,0.0f,5.0f }
    };
}



Hitbox::Hitbox()
{
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        m_ghostBox[bodyPartsNum] = new GhostObject;
    }
}

Hitbox::~Hitbox()
{

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
        // 骨の名前でボーンIDを検索
        int boneId = m_getActor->GetSkeleton().FindBoneID(BONE_NAME[bodyPartsNum]);
        // 検索したボーンIDを使用して、ボーンを取得
        Bone* bone = m_getActor->GetSkeleton().GetBone(boneId);
        // ボーンのワールド行列を取得
        Matrix boneMatrix = bone->GetWorldMatrix();

        // 行列から位置情報を取得
        Vector3 boxPos = boneMatrix.GetTranslation();
        
        // 行列から回転情報を取得
        Quaternion boxRot = Quaternion::Identity;
        boxRot.SetRotation(boneMatrix);

        // 位置を更新
        Vector3 boxPositionAdjustment = POSITION_ADJUSTMENT[bodyPartsNum];
        boxRot.Apply(boxPositionAdjustment);
        boxPos += boxPositionAdjustment;

        // 当たり判定を作成
        m_ghostBox[bodyPartsNum]->CreateBox(boxPos, boxRot, BONE_SIZE[bodyPartsNum]);

    }
}

void Hitbox::UpdateHitbox()
{
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        // 骨の名前でボーンIDを検索
        int boneId = m_getActor->GetSkeleton().FindBoneID(BONE_NAME[bodyPartsNum]);
        // 検索したボーンIDを使用して、ボーンを取得
        Bone* bone = m_getActor->GetSkeleton().GetBone(boneId);
        // ボーンのワールド行列を取得
        Matrix boneMatrix = bone->GetWorldMatrix();

        // 行列から位置情報を取得
        Vector3 boxPos = boneMatrix.GetTranslation();

        // 行列から回転情報を取得
        Quaternion boxRot = Quaternion::Identity;
        boxRot.SetRotation(boneMatrix);

        // 位置を更新
        Vector3 boxPositionAdjustment = POSITION_ADJUSTMENT[bodyPartsNum];
        boxRot.Apply(boxPositionAdjustment);
        boxPos += boxPositionAdjustment;

        // 当たり判定の情報を更新
        m_ghostBox[bodyPartsNum]->UpdateGhostObject(boxPos, boxRot);
    }
}