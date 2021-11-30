////////////////////////////////////////////////////////////
// 使用しない
////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "attack_move_bone.h"

#include "player.h"
#include "actor.h"
#include "st_attack_data.h"



namespace // constant
{
    constexpr const wchar_t* HEAD_BONE_NAME = L"J_Bip_C_Head"; // 頭の骨の名前
    constexpr const wchar_t* L_HAND_BONE_NAME = L"J_Bip_L_UpperArm"; // 左手の骨の名前
    constexpr const wchar_t* R_HAND_BONE_NAME = L"J_Bip_R_Hand"; // 右手の骨の名前
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
    // 現在攻撃中かを確認
    // 攻撃中でなければ、処理をしない
    if (false == m_getStAttackData->GetFlagAttackNow())
    {
        return;
    }

    ////////////////////////////////////////////////////////////
    // 攻撃中の骨の処理
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
    // どうにかして、どの部位を狙ってるかの情報を手に入れる

    return EnTargetBodyParts::enHead;
}

void AttackMoveBone::MoveBoneToAttackTheHead()
{
    // 現在、テスト段階では左手（モデルは反転して表示されている）で攻撃しているので、
    // 左手の骨の動きを調整する。

    Vector3 boneMove = Vector3::Zero; // 調整するボーンの移動量

    ////////////////////////////////////////////////////////////
    // ボーンの位置情報を取得
    ////////////////////////////////////////////////////////////

    // 骨の名前でボーンIDを検索
    int handBoneId = m_getActor->GetSkeleton().FindBoneID(L_HAND_BONE_NAME);
    // 検索したボーンIDを使用して、ボーンを取得
    Bone* handBone = m_getActor->GetSkeleton().GetBone(handBoneId);
    // ボーンのワールド行列を取得
    Matrix handBoneMatrix = handBone->GetWorldMatrix();
    // 行列から位置情報を取得
    Vector3 handBonePosition = handBoneMatrix.GetTranslation();

    ////////////////////////////////////////////////////////////
    // ターゲットになるボーンの位置を取得
    // ※ここでターゲットのボーンを検索して、関数を１つにしていいかも
    ////////////////////////////////////////////////////////////

    // 骨の名前でボーンIDを検索
    int targetBoneId = m_getOhterPlayer->GetActor().GetSkeleton().FindBoneID(HEAD_BONE_NAME);
    // 検索したボーンIDを使用して、ボーンを取得
    Bone* targetBone = m_getOhterPlayer->GetActor().GetSkeleton().GetBone(targetBoneId);
    // ボーンのワールド行列を取得
    Matrix targetBoneMatrix = targetBone->GetWorldMatrix();
    // 行列から位置情報を取得
    Vector3 targetBonePosition = targetBoneMatrix.GetTranslation();

    ////////////////////////////////////////////////////////////
    // ボーンの位置の調整をおこなう
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // 手のボーンとターゲットのボーンの位置の最短距離を取得して、
    // 現在の手のボーンのX、Z座標をの位置の最短距離のY座標を手のボーンのY座標にセットする
    //////////////////////////////

    // ※位置で計算しようと思ったが、
    // 手とターゲットの位置情報を使用して、上腕のボーンを回転させてターゲットに向けて攻撃する。

    Quaternion rotation = g_quatIdentity;

    rotation.SetRotation(Vector3::Front, targetBonePosition);

    Matrix rotMatrix;
    rotMatrix.MakeRotationFromQuaternion(rotation);

    // 骨の名前でボーンIDを検索
    int localhandBoneId = m_getActor->GetSkeleton().FindBoneID(L_HAND_BONE_NAME);
    // 検索したボーンIDを使用して、ボーンを取得
    Bone* localhandBone = m_getActor->GetSkeleton().GetBone(localhandBoneId);
    // ボーンのワールド行列を取得
    Matrix localhandBoneMatrix = localhandBone->GetLocalMatrix();
    // 行列から位置情報を取得
    Vector3 localhandBonePosition = localhandBoneMatrix.GetTranslation();

    localhandBoneMatrix *= rotMatrix;






    //float handVectorLength = handBonePosition.Length();
    //float targetVectorLength = targetBonePosition.Length();

    //float nowPositionLength = handVectorLength / targetVectorLength;

    //boneMove.y = targetBonePosition.y * nowPositionLength;

    //handBonePosition.y = targetBonePosition.y * nowPositionLength;



    






    //////////////////////////////
    // 最終調整、セット
    //////////////////////////////

    // とりあえずY座標の調整だけしたいのでX、Z座標を０にする
    boneMove.x = 0.0f;
    boneMove.z = 0.0f;

    //handBonePosition.Normalize();

    //handBonePosition += boneMove;

    // 調整した位置をセット
    //handBoneMatrix.SetTranslation(handBonePosition);

    // ワールド行列 = ローカル行列 * モデルのワールド行列
    // ローカル行列 = ワールド行列 / モデルのワールド行列
    // 行列には、割り算がないとのこと
    //Matrix local = handBoneMatrix / m_getActor->GetModelWorldMatrix();

    handBone->SetLocalMatrix(localhandBoneMatrix);

}

void AttackMoveBone::MoveBoneToAttackTheBody()
{

}