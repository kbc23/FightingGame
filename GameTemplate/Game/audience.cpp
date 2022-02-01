#include "stdafx.h"
#include "audience.h"

#include <random>

#include "model_render.h"

Audience::Audience()
{

}

Audience::~Audience()
{

}

bool Audience::Start()
{
    // インスタンシング描画
    InitInstancingDrawAudience();

    OffScreenRendering();

    return true;
}

void Audience::InitInstancingDrawAudience()
{

    // アニメーションをロード
    m_animationPlayer[enRooting].Load("Assets/modelData/model/rooting.tka");
    //ループ再生をtrueにする
    m_animationPlayer[enRooting].SetLoopFlag(true);

    // モデルの初期化
    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->InstancingInit(
        "Assets/modelData/model/model.tkm",
        m_AUDIENCE_NUM,
        modelUpAxis::enModelUpAxisZ,
        m_animationPlayer,
        AnimationMax
    );

    // モデルの拡大率を２倍にする
    m_modelCharacter->SetAllInstancScale({ 2.0f,2.0f,2.0f });

    // 位置を決める
    SetPosition();

    // 応援のアニメーションを再生
    //m_modelCharacter->PlayAnimation(enRooting);

    //SetRandomAnimationStartTime();
}

void Audience::SetPosition()
{
    // いったん10×3をリング４方向に作成で作業
    // 外周４００ぐらいに作成

    int instanceNum = 0;

    //////////////////////////////
    // グループ１
    //////////////////////////////

    float verticalPosition = 400.0f; // グループ内での位置（縦）
    float besidePosition = 25.0f; // グループ内での位置（横）

    for (int verticalNum = 0; verticalNum < m_AUDIENCE_GROUP_VERTICAL_NUM; verticalNum++) {
        for (int besideNum = 0; besideNum < m_AUDIENCE_GROUP_BESIDE_NUM; besideNum++) {
            // besideNum == 偶数
            if (0 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetInstancePosition(
                    instanceNum,
                    { besidePosition,0.0f,verticalPosition }
                );
            }
            // besideNum == 奇数（終わった後に、besidePositionを+50）
            else if (1 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetInstancePosition(
                    instanceNum,
                    { -besidePosition,0.0f,verticalPosition }
                );

                besidePosition += 50.0f;
            }

            m_modelCharacter->SetInstanceRotation(instanceNum, 180.0f);

            ++instanceNum;
        }

        verticalPosition += 50.0f;
        besidePosition = 25.0f;
    }

    //////////////////////////////
    // グループ２
    //////////////////////////////

    verticalPosition = -400.0f;
    besidePosition = 25.0f;

    for (int verticalNum = 0; verticalNum < m_AUDIENCE_GROUP_VERTICAL_NUM; verticalNum++) {
        for (int besideNum = 0; besideNum < m_AUDIENCE_GROUP_BESIDE_NUM; besideNum++) {
            // besideNum == 偶数
            if (0 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetInstancePosition(
                    instanceNum,
                    { besidePosition,0.0f,verticalPosition }
                );
            }
            // besideNum == 奇数（終わった後に、besidePositionを+50）
            else if (1 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetInstancePosition(
                    instanceNum,
                    { -besidePosition,0.0f,verticalPosition }
                );

                besidePosition += 50.0f;
            }

            m_modelCharacter->SetInstanceRotation(instanceNum, 0.0f);

            ++instanceNum;
        }

        verticalPosition += 50.0f;
        besidePosition = 25.0f;
    }

    //////////////////////////////
    // グループ３
    //////////////////////////////

    verticalPosition = 25.0f;
    besidePosition = 400.0f;

    for (int verticalNum = 0; verticalNum < m_AUDIENCE_GROUP_VERTICAL_NUM; verticalNum++) {
        for (int besideNum = 0; besideNum < m_AUDIENCE_GROUP_BESIDE_NUM; besideNum++) {
            // besideNum == 偶数
            if (0 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetInstancePosition(
                    instanceNum,
                    { besidePosition,0.0f,verticalPosition }
                );
            }
            // besideNum == 奇数（終わった後に、verticalPositionを+50）
            else if (1 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetInstancePosition(
                    instanceNum,
                    { besidePosition,0.0f,-verticalPosition }
                );

                verticalPosition += 50.0f;
            }

            m_modelCharacter->SetInstanceRotation(instanceNum, -90.0f);

            ++instanceNum;
        }

        besidePosition += 50.0f;
        verticalPosition = 25.0f;
    }

    //////////////////////////////
    // グループ４
    //////////////////////////////

    verticalPosition = 25.0f;
    besidePosition = -400.0f;

    for (int verticalNum = 0; verticalNum < m_AUDIENCE_GROUP_VERTICAL_NUM; verticalNum++) {
        for (int besideNum = 0; besideNum < m_AUDIENCE_GROUP_BESIDE_NUM; besideNum++) {
            // besideNum == 偶数
            if (0 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetInstancePosition(
                    instanceNum,
                    { besidePosition,0.0f,verticalPosition }
                );
            }
            // besideNum == 奇数（終わった後に、verticalPositionを+50）
            else if (1 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetInstancePosition(
                    instanceNum,
                    { besidePosition,0.0f,-verticalPosition }
                );

                verticalPosition += 50.0f;
            }

            m_modelCharacter->SetInstanceRotation(instanceNum, 90.0f);

            ++instanceNum;
        }

        besidePosition += 50.0f;
        verticalPosition = 25.0f;
    }
}

void Audience::SetRandomAnimationStartTime()
{
    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<int> dist(1, 60);

    for (int audienceNum = 0; audienceNum < m_AUDIENCE_NUM; audienceNum++) {
        m_animationStartTime[audienceNum] = dist(mt);
    }
}

void Audience::OffScreenRendering()
{
    // アニメーションをロード
    m_animationPlayer[enRooting].Load("Assets/modelData/model/rooting.tka");
    //ループ再生をtrueにする
    m_animationPlayer[enRooting].SetLoopFlag(true);

    // モデルの初期化
    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->OffScreenInit(
        "Assets/modelData/model/model.tkm",
        m_AUDIENCE_NUM,
        modelUpAxis::enModelUpAxisZ
        //m_animationPlayer,
        //AnimationMax
    );

    // モデルの拡大率を２倍にする
    m_modelCharacter->SetScale({ 2.0f,2.0f,2.0f });

    // 位置を決める
    SetOffScreenPosition();

    // 応援のアニメーションを再生
    //m_modelCharacter->PlayAnimation(enRooting);
}

void Audience::Update()
{
    //RandomAnimationStart();
}

void Audience::RandomAnimationStart()
{
    if (m_animationStartTimer > 60) {
        return;
    }

    for (int audienceNum = 0; audienceNum < m_AUDIENCE_NUM; audienceNum++) {
        if (true == m_flagAnimationStart[audienceNum]) {
            continue;
        }

        if (m_animationStartTime[audienceNum] < m_animationStartTimer) {
            // 応援のアニメーションを再生
            m_modelCharacter->PlayAnimation(enRooting);

            m_flagAnimationStart[audienceNum] = true;
        }
    }

    ++m_animationStartTimer;
}

void Audience::SetOffScreenPosition()
{
    // いったん10×3をリング４方向に作成で作業
    // 外周４００ぐらいに作成

    int instanceNum = 0;

    //////////////////////////////
    // グループ１
    //////////////////////////////

    float verticalPosition = 400.0f; // グループ内での位置（縦）
    float besidePosition = 25.0f; // グループ内での位置（横）

    for (int verticalNum = 0; verticalNum < m_AUDIENCE_GROUP_VERTICAL_NUM; verticalNum++) {
        for (int besideNum = 0; besideNum < m_AUDIENCE_GROUP_BESIDE_NUM; besideNum++) {
            // besideNum == 偶数
            if (0 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetOffScreenPosition(
                    instanceNum,
                    { besidePosition,0.0f,verticalPosition }
                );
            }
            // besideNum == 奇数（終わった後に、besidePositionを+50）
            else if (1 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetOffScreenPosition(
                    instanceNum,
                    { -besidePosition,0.0f,verticalPosition }
                );

                besidePosition += 50.0f;
            }

            m_modelCharacter->SetOffScreenRotation(instanceNum, 180.0f);

            ++instanceNum;
        }

        verticalPosition += 50.0f;
        besidePosition = 25.0f;
    }

    //////////////////////////////
    // グループ２
    //////////////////////////////

    verticalPosition = -400.0f;
    besidePosition = 25.0f;

    for (int verticalNum = 0; verticalNum < m_AUDIENCE_GROUP_VERTICAL_NUM; verticalNum++) {
        for (int besideNum = 0; besideNum < m_AUDIENCE_GROUP_BESIDE_NUM; besideNum++) {
            // besideNum == 偶数
            if (0 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetOffScreenPosition(
                    instanceNum,
                    { besidePosition,0.0f,verticalPosition }
                );
            }
            // besideNum == 奇数（終わった後に、besidePositionを+50）
            else if (1 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetOffScreenPosition(
                    instanceNum,
                    { -besidePosition,0.0f,verticalPosition }
                );

                besidePosition += 50.0f;
            }

            m_modelCharacter->SetOffScreenRotation(instanceNum, 0.0f);

            ++instanceNum;
        }

        verticalPosition += 50.0f;
        besidePosition = 25.0f;
    }

    //////////////////////////////
    // グループ３
    //////////////////////////////

    verticalPosition = 25.0f;
    besidePosition = 400.0f;

    for (int verticalNum = 0; verticalNum < m_AUDIENCE_GROUP_VERTICAL_NUM; verticalNum++) {
        for (int besideNum = 0; besideNum < m_AUDIENCE_GROUP_BESIDE_NUM; besideNum++) {
            // besideNum == 偶数
            if (0 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetOffScreenPosition(
                    instanceNum,
                    { besidePosition,0.0f,verticalPosition }
                );
            }
            // besideNum == 奇数（終わった後に、verticalPositionを+50）
            else if (1 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetOffScreenPosition(
                    instanceNum,
                    { besidePosition,0.0f,-verticalPosition }
                );

                verticalPosition += 50.0f;
            }

            m_modelCharacter->SetOffScreenRotation(instanceNum, -90.0f);

            ++instanceNum;
        }

        besidePosition += 50.0f;
        verticalPosition = 25.0f;
    }

    //////////////////////////////
    // グループ４
    //////////////////////////////

    verticalPosition = 25.0f;
    besidePosition = -400.0f;

    for (int verticalNum = 0; verticalNum < m_AUDIENCE_GROUP_VERTICAL_NUM; verticalNum++) {
        for (int besideNum = 0; besideNum < m_AUDIENCE_GROUP_BESIDE_NUM; besideNum++) {
            // besideNum == 偶数
            if (0 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetOffScreenPosition(
                    instanceNum,
                    { besidePosition,0.0f,verticalPosition }
                );
            }
            // besideNum == 奇数（終わった後に、verticalPositionを+50）
            else if (1 == besideNum % 2) {
                // 位置をセット
                m_modelCharacter->SetOffScreenPosition(
                    instanceNum,
                    { besidePosition,0.0f,-verticalPosition }
                );

                verticalPosition += 50.0f;
            }

            m_modelCharacter->SetOffScreenRotation(instanceNum, 90.0f);

            ++instanceNum;
        }

        besidePosition += 50.0f;
        verticalPosition = 25.0f;
    }
}
