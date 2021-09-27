#include "stdafx.h"
#include "model_render.h"

#include "shadow.h"
#include "shadow_map.h"
#include "shadow_light_camera.h"

ModelRender::ModelRender()
{

}

ModelRender::~ModelRender()
{

}

bool ModelRender::Start()
{


	return true;
}

////////////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////////////

//void ModelRender::Init(const char* filePath)
//{
//  //tkm�t�@�C���̃t�@�C���p�X��ێ�
//  m_tkmFilePath = filePath;
//
//	//���f���̏�����
//	InitModel(filePath);
//
//	//����������
//	m_finishInit = true;
//}

void ModelRender::Init(const char* filePath,
	bool flagShadow,
	modelUpAxis::EnModelUpAxis modelUpAxis,
	AnimationClip* animationClip,
	int maxAnimationClipNum
)
{
	//tkm�t�@�C���̃t�@�C���p�X��ێ�
	m_tkmFilePath = filePath;
	//�X�P���g���̃f�[�^�̓ǂݍ���
	InitSkeleton(filePath);
	//���f���̏�����
	InitModel(filePath, flagShadow, modelUpAxis);
	//�A�j���[�V������������
	InitAnimation(animationClip, maxAnimationClipNum);

	//����������
	m_finishInit = true;
}

bool ModelRender::InitSkeleton(const char* filePath)
{
	//tkm�t�@�C����tks�t�@�C���ɕϊ�
	std::string skeletonFilePath = filePath;
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");

	//�X�P���g���̃��\�[�X���m��
	m_skeletonPointer.reset(new Skeleton);
	//�X�P���g���̃f�[�^�̓ǂݍ���
	bool isInited = m_skeletonPointer->Init(skeletonFilePath.c_str());

	//�������ɐ���������
	//����
	if (isInited) {
		return true;
	}
	//���s
	else {
		//�X�P���g���̃��\�[�X�����
		m_skeletonPointer.reset();
		return false;
	}
}

void ModelRender::InitAnimation(AnimationClip* animationClip, int maxAnimationClipNum)
{
	//�A�j���\�V�����N���b�v��o�^���Ă��邩
	if (animationClip == nullptr) {
		//���Ă�����return
		return;
	}

	//�A�j���[�V�����̃��\�[�X���m��
	m_animationPointer.reset(new Animation);
	//�A�j���[�V������������
	m_animationPointer->Init(
		*m_skeletonPointer,
		animationClip,
		maxAnimationClipNum
	);
}

void ModelRender::InitModel(const char* filePath,
	bool flagShadow,
	modelUpAxis::EnModelUpAxis modelUpAxis
)
{
	//���C�g�̐ݒ������B
	//�f�B���N�V�������C�g������������
	InitDirectionLight();

	//�|�C���g���C�g������������
	//InitPointLight();

	//�X�|�b�g���C�g������������
	//InitSpotLight();

	//����������������
	InitAmbientLight();

	//�������C�g������������
	//InitHemiLight();

	//3D���f�������[�h���邽�߂̏���ݒ肷��
	//���f���̏��������邽�߂̏���ݒ�
	ModelInitData modelInitData;
	//tkm�t�@�C���̃t�@�C���p�X��ݒ�
	modelInitData.m_tkmFilePath = filePath;
	//�g�p����V�F�[�_�[�t�@�C���p�X��ݒ�
	if (false == flagShadow) {
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";

		//���C�g�̏���萔�o�b�t�@�Ƃ��ăf�B�X�N���v�^�q�[�v��
		//�o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn���B
		modelInitData.m_expandConstantBuffer = &m_light;
		modelInitData.m_expandConstantBufferSize = sizeof(m_light);
	}
	else {
		modelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";
		//�V���h�E�}�b�v���g��SRV�ɐݒ肷��B
		modelInitData.m_expandShaderResoruceView = 
			&ShadowMap::GetInstance()->GetShadowMap().GetRenderTargetTexture();

		//���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�ɐݒ肷��B
		modelInitData.m_expandConstantBuffer =
			(void*)&ShadowLightCamera::GetInstance()->GetShadowLightCamera().GetViewProjectionMatrix();
		modelInitData.m_expandConstantBufferSize =
			sizeof(ShadowLightCamera::GetInstance()->GetShadowLightCamera().GetViewProjectionMatrix());
	}
	//�X�P���g�����w�肷��B
	if (m_skeletonPointer) {	//�X�P���g��������������Ă�����
		modelInitData.m_skeleton = m_skeletonPointer.get();
	}
	//���f���̏�������w��
	modelInitData.m_modelUpAxis = modelUpAxis;

	//�����������g���ă��f���\������������������
	m_model.Init(modelInitData);
}

//////////////////////////////
// ���C�g���Ƃ̏�����
//////////////////////////////

void ModelRender::InitDirectionLight()
{
	//���C�g�͎΂ߏォ�瓖�����Ă���B
	m_light.dirDirection.x = 1.0f;
	m_light.dirDirection.y = -1.0f;
	m_light.dirDirection.z = -1.0f;
	//���K������B
	m_light.dirDirection.Normalize();
	//���C�g�̃J���[�̐ݒ�i���C�g�̋����j
	m_light.dirColor.x = 1.5f;
	m_light.dirColor.y = 1.5f;
	m_light.dirColor.z = 1.5f;

	//���_�̈ʒu��ݒ�
	m_light.eyePos = g_camera3D->GetPosition();
}


void ModelRender::InitPointLight()
{
	//�|�C���g���C�g�̏������W��ݒ肷��
	m_light.ptPosition.x = 0.0f;
	m_light.ptPosition.y = 50.0f;
	m_light.ptPosition.z = 50.0f;

	//�|�C���g���C�g�̏����J���[��ݒ肷��
	m_light.ptColor.x = 15.0f;
	m_light.ptColor.y = 0.0f;
	m_light.ptColor.z = 240.0f;

	//�|�C���g���C�g�̉e���͈͂�ݒ肷��
	m_light.ptRange = 100.0f;
}

void ModelRender::InitSpotLight()
{
	//�������W
	m_light.spPosition.x = 0.0f;
	m_light.spPosition.y = 50.0f;
	m_light.spPosition.z = -150.0f;

	//���C�g�̃J���[
	m_light.spColor.x = 200.0f;
	m_light.spColor.y = 10.0f;
	m_light.spColor.z = 10.0f;
	//���������͎΂߉��ɂ���B
	m_light.spDirection.x = 1.0f;
	m_light.spDirection.y = -1.0f;
	m_light.spDirection.z = 1.0f;
	//���K��
	m_light.spDirection.Normalize();
	//�ˏo�͈͂�300
	m_light.spRange = 300.0f;
	//�ˏo�p�x��25�x
	m_light.spAngle = Math::DegToRad(25.0f);
}

void ModelRender::InitAmbientLight()
{
	//����
	m_light.ambientLight.x = 0.05f;
	m_light.ambientLight.y = 0.05f;
	m_light.ambientLight.z = 0.05f;
}

void ModelRender::InitHemiLight()
{
	//�n�ʐF�A�V���F�A�n�ʂ̖@���̃f�[�^��ݒ肷��
	m_light.groundColor.x = 0.7f;
	m_light.groundColor.y = 0.5f;
	m_light.groundColor.z = 0.3f;

	//�V���F��ݒ�
	m_light.skyColor.x = 0.15f;
	m_light.skyColor.y = 0.7f;
	m_light.skyColor.z = 0.95f;

	//�n�ʂ̖@����ݒ�
	m_light.groundNormal.x = 0.0f;
	m_light.groundNormal.y = 1.0f;
	m_light.groundNormal.z = 0.0f;
}

////////////////////////////////////////////////////////////
// �`�揈��
////////////////////////////////////////////////////////////

void ModelRender::Render(RenderContext& renderContext)
{
	//����������
	if (m_finishInit == false) {
		return;
	}

	//���f���̕`��
	m_model.Draw(renderContext);
}

void ModelRender::Update()
{
	//����������
	if (m_finishInit == false) {
		return;
	}

	//�X�P���g�����X�V�B
	if (m_skeletonPointer) {	//�X�P���g��������������Ă�����
		m_skeletonPointer->Update(m_model.GetWorldMatrix());
	}
	//�A�j���[�V������i�߂�B
	if (m_animationPointer) {	//�A�j���[�V����������������Ă�����
		m_animationPointer->Progress(g_gameTime->GetFrameDeltaTime());
	}
	//���f���̍��W�X�V
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}