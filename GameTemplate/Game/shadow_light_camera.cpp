#include "stdafx.h"
#include "shadow_light_camera.h"

ShadowLightCamera* ShadowLightCamera::m_instance = nullptr;

void ShadowLightCamera::CreateShadowLightCamera()
{
	//カメラの位置を設定。これはライトの位置。
	m_shadowLightCamera.SetPosition(0.0f, 600.0f, 0.0f);
	//カメラの注視点を設定。これがライトが照らしている場所。
	m_shadowLightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	//【注目】上方向を設定。今回はライトが真下を向いているので、X方向を上にしている。
	m_shadowLightCamera.SetUp({ 1.0f, 0.0f, 0.0f });
	//今回のサンプルでは画角を狭めにしておく。
	m_shadowLightCamera.SetViewAngle(Math::DegToRad(20.0f));
	//ライトビュープロジェクション行列を計算している。
	m_shadowLightCamera.Update();
}