#include "stdafx.h"
#include "hitbox.h"

#include "player.h"
#include "actor.h"
#include "st_attack_data.h"
#include "st_defense_data.h"



namespace // constant
{
    // モデルは左右反転している

    // 骨の名前
    constexpr const wchar_t* BONE_NAME[Hitbox::m_MAX_BODY_PARTS] =
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

    // 当たり判定のサイズ
    const Vector3 HITBOX_SIZE[Hitbox::m_MAX_BODY_PARTS] =
    {
        { 20.0f,20.0f,20.0f },  // 頭
        { 25.0f,30.0f,20.0f },  // 身体
        { 25.0f,10.0f,20.0f },  // 尻
        // 左
        { 20.0f,6.0f,6.0f },    // 上腕
        { 17.0f,6.0f,6.0f },    // 前腕
        { 8.0f,8.0f,8.0f },     // 手
        { 12.0f,30.0f,12.0f },  // 太もも
        { 12.0f,30.0f,12.0f },  // 下腿
        { 8.0f,8.0f,18.0f },    // 足
        // 右
        { 20.0f,6.0f,6.0f },    // 上腕
        { 17.0f,6.0f,6.0f },    // 前腕
        { 8.0f,8.0f,8.0f },     // 手
        { 12.0f,30.0f,12.0f },  // 太もも
        { 12.0f,30.0f,12.0f },  // 下腿
        { 8.0f,8.0f,18.0f }     // 足
    };

    // 当たり判定の位置調整
    const Vector3 HITBOX_POSITION_ADJUSTMENT[Hitbox::m_MAX_BODY_PARTS] =
    {
        { 0.0f,8.0f,0.0f },     // 頭
        { 0.0f,0.0f,0.0f },     // 身体
        { 0.0f,-5.0f,0.0f },    // 尻
        // 左
        { 10.0f,0.0f,0.0f },    // 上腕
        { 12.0f,0.0f,0.0f },    // 前腕
        { 5.0f,0.0f,0.0f },     // 手
        { 0.0f,-15.0f,0.0f },   // 太もも
        { 0.0f,-15.0f,-2.0f },   // 下腿
        { 0.0f,0.0f,5.0f },     // 足
        // 右
        { -10.0f,0.0f,0.0f },   // 上腕
        { -12.0f,0.0f,0.0f },   // 前腕
        { -5.0f,0.0f,0.0f },    // 手
        { 0.0f,-15.0f,0.0f },   // 太もも
        { 0.0f,-15.0f,-2.0f },   // 下腿
        { 0.0f,0.0f,5.0f }      // 足
    };
}



Hitbox::Hitbox()
{
    // 身体のパーツすべてが入る配列のサイズを確保
    m_ghostBox.resize(EnBodyParts::enMaxBodyParts);

    // ポインタを作成
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        m_ghostBox[bodyPartsNum].reset(new GhostObject);
    }
}

Hitbox::~Hitbox()
{
    // ポインタを削除
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
    // 引数で持ってきた物をこのクラスで保持
    m_getOtherPlayer = &otherPlayer;
    m_getActor = &actor;
    m_getStAttackData = &attackData;
    m_getStDefenseData = &defenseData;

    // キャラクターのModelRenderクラスで位置情報の更新をしたいため、
    // ここでModelRenderクラスに自身の情報を渡す
    // （関数名変えること）
    m_getActor->SetTest(*this);

    // ボックスの作成
    //Create();

    m_flagInit = true;
}

void Hitbox::Update()
{

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

        // 位置の調整
        Vector3 boxPositionAdjustment = HITBOX_POSITION_ADJUSTMENT[bodyPartsNum];
        boxRot.Apply(boxPositionAdjustment);
        boxPos += boxPositionAdjustment;

        // 当たり判定を作成
        m_ghostBox[bodyPartsNum]->CreateBox(boxPos, boxRot, HITBOX_SIZE[bodyPartsNum]);
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

        // 位置の調整
        Vector3 boxPositionAdjustment = HITBOX_POSITION_ADJUSTMENT[bodyPartsNum];
        boxRot.Apply(boxPositionAdjustment);
        boxPos += boxPositionAdjustment;

        // 当たり判定の情報を更新
        m_ghostBox[bodyPartsNum]->UpdateGhostObject(boxPos, boxRot);
    }
}

const bool Hitbox::UpdateCheckAttack()
{
    // 攻撃中ではない
    if (false == m_getStAttackData->GetFlagAttackNow()) {
        return false;
    }
    
    // すでに攻撃が当たっている
    if (true == m_getStAttackData->GetFlagAlreadyAttacked()) {
        return false;
    }

    // 攻撃が当たったか
    int hitBodyParts = CheckHit();
    if (EnBodyParts::enMaxBodyParts != hitBodyParts) {
        // 攻撃が当たったときの処理
        HitAttack();

        // ここで防御してるか、当たった場所が手、前腕かの確認をおこなう
        if (true == m_getOtherPlayer->GetStDefenseData().GetFlagDefense() &&
            true == CheckHitDefenseBodyParts(hitBodyParts)) {
            // 攻撃は当たったけど、ダメージはない
            return false;
        }

        // 攻撃が当たった
        return true;
    }

    // 攻撃は当たってない
    return false;
}

const int Hitbox::CheckHit()
{
    int checkHitBodyParts = EnBodyParts::enMaxBodyParts; // 当たり判定と攻撃判定が触れているか

    //プレイヤーの当たり判定と攻撃判定が触れたかの判定
    for (int bodyPartsNum = 0; EnBodyParts::enMaxBodyParts > bodyPartsNum; ++bodyPartsNum) {
        PhysicsWorld::GetInstance()->ContactTest(&m_getOtherPlayer->GetGhostObject(bodyPartsNum),
            [&](const btCollisionObject& contactObject)
            {
                // 左手が当たったとき
                if (m_ghostBox[EnBodyParts::enLeftHand]->IsSelf(contactObject)) {
                    checkHitBodyParts = bodyPartsNum;
                    return;
                }
            });

        // 当たったら判定処理から抜ける
        if (EnBodyParts::enMaxBodyParts != checkHitBodyParts) {
            break;
        }
    }

    return checkHitBodyParts;
}

const bool Hitbox::CheckHitDefenseBodyParts(const int bodyParts)
{
    // 手に当たったか
    if (EnBodyParts::enLeftHand == bodyParts || EnBodyParts::enRightHand == bodyParts) {
        return true;
    }
    // 前腕に当たったか
    if (EnBodyParts::enLeftForearm == bodyParts || EnBodyParts::enRightForearm == bodyParts) {
        return true;
    }

    // 当たっていない場合
    return false;
}

void Hitbox::HitAttack()
{
    // すでに攻撃が当たったことを伝える
    m_getStAttackData->SetFlagAlreadyAttacked(true);
}