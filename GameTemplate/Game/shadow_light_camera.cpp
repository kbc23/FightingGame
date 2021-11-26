#include "stdafx.h"
#include "shadow_light_camera.h"

ShadowLightCamera* ShadowLightCamera::m_instance = nullptr;

void ShadowLightCamera::CreateShadowLightCamera()
{
	// �J�����̈ʒu��ݒ�
	m_shadowLightCamera.SetPosition(0.0f, 600.0f, 0.0f);
	// �J�����̒����_��ݒ�
	m_shadowLightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	// �J�����̏������ݒ�
	m_shadowLightCamera.SetUp({ 1.0f, 0.0f, 0.0f });
	// �J�����̉�p��ݒ�
	m_shadowLightCamera.SetViewAngle(Math::DegToRad(1000.0f));
	// ���C�g�r���[�v���W�F�N�V�����s����v�Z
	m_shadowLightCamera.Update();
}