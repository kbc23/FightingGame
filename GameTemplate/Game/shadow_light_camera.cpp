#include "stdafx.h"
#include "shadow_light_camera.h"

ShadowLightCamera* ShadowLightCamera::m_instance = nullptr;

void ShadowLightCamera::CreateShadowLightCamera()
{
	// カメラの位置を設定
	m_shadowLightCamera.SetPosition(0.0f, 600.0f, 0.0f);
	// カメラの注視点を設定
	m_shadowLightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	// カメラの上方向を設定
	m_shadowLightCamera.SetUp({ 1.0f, 0.0f, 0.0f });
	// カメラの画角を設定
	m_shadowLightCamera.SetViewAngle(Math::DegToRad(1000.0f));
	// ライトビュープロジェクション行列を計算
	m_shadowLightCamera.Update();
}