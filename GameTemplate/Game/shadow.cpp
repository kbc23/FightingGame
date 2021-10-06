#include "stdafx.h"
#include "shadow.h"
#include "shadow_light_camera.h"

Shadow::Shadow()
{

}

Shadow::~Shadow()
{

}

bool Shadow::Start()
{
    return true;
}

void Shadow::Update()
{
    //未初期化時
    if (m_finishInit == false) {
        return;
    }

    //モデルの座標更新
    m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void Shadow::ShadowRender(RenderContext& renderContext)
{
    //未初期化時
    if (m_finishInit == false) {
        return;
    }

    //モデルの描画
    m_shadowModel.Draw(renderContext, ShadowLightCamera::GetInstance()->GetShadowLightCamera());
}

void Shadow::Init(const char* filePath)
{
    // step-3 シャドウマップ描画用のモデルを用意する
    ModelInitData ShadowModelInitData;
    ShadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
    ShadowModelInitData.m_tkmFilePath = filePath;
    m_shadowModel;
    m_shadowModel.Init(ShadowModelInitData);
    m_shadowModel.UpdateWorldMatrix(
        { 0,50,0 },
        g_quatIdentity,
        g_vec3One
    );

    m_finishInit = true;
}