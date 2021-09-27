#include "stdafx.h"
#include "shadow_light_camera.h"

ShadowLightCamera* ShadowLightCamera::m_instance = nullptr;

void ShadowLightCamera::CreateShadowLightCamera()
{
	//�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
	m_shadowLightCamera.SetPosition(0.0f, 600.0f, 0.0f);
	//�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
	m_shadowLightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	//�y���ځz�������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���B
	m_shadowLightCamera.SetUp({ 1.0f, 0.0f, 0.0f });
	//����̃T���v���ł͉�p�����߂ɂ��Ă����B
	m_shadowLightCamera.SetViewAngle(Math::DegToRad(20.0f));
	//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
	m_shadowLightCamera.Update();
}