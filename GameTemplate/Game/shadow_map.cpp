#include "stdafx.h"
#include "shadow_map.h"
#include "../../MiniEngine/RenderTarget.h"

ShadowMap* ShadowMap::m_instance = nullptr;

void ShadowMap::CreateShadowMap()
{
    // シャドウマップ描画用のレンダリングターゲットを作成する
    float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    m_shadowMap.Create(
        1024,
        1024,
        1,
        1,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_D32_FLOAT,
        clearColor
    );
}