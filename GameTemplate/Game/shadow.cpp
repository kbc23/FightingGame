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
    // ����������
    if (m_finishInit == false) {
        return;
    }

    // ���f���̍��W�X�V
    m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void Shadow::ShadowRender(RenderContext& renderContext)
{
    // ����������
    if (m_finishInit == false) {
        return;
    }

    // ���f���̕`��
    m_shadowModel.Draw(renderContext, ShadowLightCamera::GetInstance()->GetShadowLightCamera());
}

void Shadow::Init(const char* filePath, Skeleton& skeleton)
{
    // �V���h�E�}�b�v�`��p�̃��f����p�ӂ���
    ModelInitData ShadowModelInitData;
    ShadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
    ShadowModelInitData.m_tkmFilePath = filePath;
    // �X�P���g�����w�肷��B
    if (&skeleton) {	// �X�P���g��������������Ă�����
        ShadowModelInitData.m_skeleton = &skeleton;
    }

    m_shadowModel.Init(ShadowModelInitData);
    m_shadowModel.UpdateWorldMatrix(
        { 0,50,0 },
        g_quatIdentity,
        g_vec3One
    );

    m_finishInit = true;
}