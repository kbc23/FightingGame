#pragma once
#include <Model.h>

#include "shadow.h"
#include "hitbox.h"

////////////////////////////////////////////////////////////
// ����
// �g�D�[�������_�����O�Ɏg�p����摜�Ȃǂ̃p�X��ǂݍ���ł���Ƃ���
// NullTextureMaps.cpp
////////////////////////////////////////////////////////////



// ���C�g�p�̍\���̂��`����
struct Light {
	// �f�B���N�V�������C�g
	Vector3 dirDirection; // ���C�g�̕���
	// HLSL���̒萔�o�b�t�@��float3�^�̕ϐ���16�̔{���̃A�h���X�ɔz�u����邽�߁A
	// C++���ɂ̓p�f�B���O�𖄂߂Ă����B
	float pad;
	Vector3 dirColor;
	float pad1;

	// �|�C���g���C�g
	Vector3 ptPosition; // �ʒu
	float pad2;			// �p�f�B���O
	Vector3 ptColor;	// �J���[
	float ptRange;		// �e���͈�

	// �X�|�b�g���C�g
	Vector3 spPosition;		// �ʒu
	float pad3;				// �p�f�B���O
	Vector3 spColor;		// �J���[
	float spRange;			// �e���͈�
	Vector3 spDirection;	// �ˏo����
	float spAngle;			// �ˏo�p�x

	// �\���̂Ɏ��_�̈ʒu��ǉ�����
	Vector3 eyePos; // ���_�̈ʒu
	float pad4;

	// ����
	Vector3 ambientLight;   // �A���r�G���g���C�g
	float pad5;

	// �������C�g
	Vector3 groundColor;	// �n�ʐF
	float pad6;
	Vector3 skyColor;		// �V���F
	float pad7;
	Vector3 groundNormal;	// �n�ʂ̖@��

	float pad8;
	Matrix shadowCamera;
};



class ModelRender : public IGameObject
{
public:
    ModelRender();
    ~ModelRender();
	bool Start() override final;
    void Update() override final;
	void Render(RenderContext& renderContext) override final;


public:
	/**
	 * @brief ������
	 * @param filePath tkm�t�@�C���̃t�@�C���p�X
	 * @param flagShadowReceiver �e�̉e�����󂯂邩
	 * @param flagShadow �e��`�悷�邩
	 * @param modelUpAxis ���f���̏����
	 * @param animationClip �A�j���[�V�����N���b�v
	 * @param maxAnimationClipNum �A�j���[�V�����N���b�v�̍ő吔
	 */
	void Init(const char* filePath, bool flagShadowReceiver = false, bool flagShadow = false,
		modelUpAxis::EnModelUpAxis modelUpAxis = modelUpAxis::enModelUpAxisZ,
		AnimationClip* animationClip = nullptr,
		int maxAnimationClipNum = 0
	);


private:
	/**
	 * @brief ���f���̏�����
	 * @param filePath tkm�t�@�C���̃t�@�C���p�X
	 * @param flagShadowReceiver �e�̉e�����󂯂邩
	 * @param flagShadow �e��`�悷�邩
	 * @param modelUpAxis ���f���̏����
	*/
	void InitModel(const char* filePath, bool flagShadowReceiver, bool flagShadow,
		modelUpAxis::EnModelUpAxis modelUpAxis
	);

	/**
	 * @brief �f�B���N�V�������C�g�̏�����
	*/
	void InitDirectionLight();

	/**
	 * @brief �|�C���g���C�g�̏�����
	*/
	void InitPointLight();

	/**
	 * @brief �X�|�b�g���C�g�̏�����
	*/
	void InitSpotLight();

	/**
	 * @brief �����̏�����
	*/
	void InitAmbientLight();

	/**
	 * @brief �������C�g�̏�����
	*/
	void InitHemiLight();

	/**
	 * @brief �X�P���g���̏�����
	 * @param filePath �t�@�C���p�X
	 * @return 
	*/
	bool InitSkeleton(const char* filePath);

	/**
	 * @brief �A�j���[�V�����̏�����
	 * @param animationClip �A�j���[�V�����N���b�v
	 * @param maxAnimationClipNum �A�j���[�V�����N���b�v�̑���
	*/
	void InitAnimation(AnimationClip* animationClip, int maxAnimationClipNum);

	/**
	 * @brief ���̃N���X�̕������A���f���ƈʒu�������̂�h�����߂ɂ����ŏ����������Ȃ�
	*/
	void UpdateOtherData();


private:
	/**
	 * @brief �X�E�F�[�̏���
	*/
	void SwayCharacter();


	void CheckSwayMove();

public:
	/**
	 * @brief �A�j���[�V�������Đ�����
	 * @param animationNumber �o�^���Ă���A�j���[�V�����N���b�v�̔ԍ�
	 * @param interpolateTime �⊮���ԁi�P�ʁF�b�j
	*/
	void PlayAnimation(int animationNumber, float interpolateTime = 0.2f)
	{
		m_animationPointer->Play(animationNumber, interpolateTime);
	}

	/**
	 * @brief �A�j���[�V�������Đ�����
	 * @return �Đ�����
	*/
	const bool IsPlayingAnimation() const
	{
		return m_animationPointer->IsPlaying();
	}

	/**
	 * @brief �A�j���[�V�����̏��������������Ă��邩
	 * @return �������Ă��邩
	*/
	const bool IsInited() const
	{
		return m_animationPointer->IsInited();
	}


public: // Get�֐�
	/**
	 * @brief �ʒu���擾
	 * @return �ʒu
	*/
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/**
	 * @brief ��]�ʂ��擾
	 * @return ��]��
	*/
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/**
	 * @brief �g�嗦���擾
	 * @return �g�嗦
	*/
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/**
	 * @brief �w�肵���{�[�����擾
	 * @param boneName �{�[���̖��O
	 * @return �{�[��
	*/
	Bone& GetBone(const wchar_t boneName) const
	{
		// �{�[���̖��O���w�肵�ă{�[��ID������
		int boneId = m_skeletonPointer->FindBoneID(&boneName);
		// ���������{�[��ID���g�p���āA�{�[�����擾
		return *m_skeletonPointer->GetBone(boneId);
	}

	/**
	 * @brief �X�P���g�����擾
	 * @return �X�P���g��
	*/
	Skeleton& GetSkeleton() const
	{
		return *m_skeletonPointer;
	}

	Matrix& GetModelWorldMatrix() const
	{
		m_model.GetWorldMatrix();
	}


public: // Set�֐�
	/**
	 * @brief �ʒu��ݒ�
	 * @param vec �ʒu
	*/
	void SetPosition(const Vector3& position)
	{
		m_position = position;

		if (true == m_flagShadow) {
			m_shadowModel->SetPosition(m_position);
		}
	}

	/**
	 * @brief Y���W���w�肵���ʂ����㏸
	 * @param position �㏸�����
	*/
	void UpPositionY(const float position)
	{
		m_position.y += position;
	}

	/**
	 * @brief Y���W���w�肵���ʂ������~
	 * @param position ���~�����
	*/
	void DownPositionY(const float position)
	{
		m_position.y -= position;
	}

	/**
	 * @brief Y���W��0.0f�ɐݒ肷��
	*/
	void ResetPositionY()
	{
		m_position.y = 0.0f;
	}

	/**
	 * @brief ��]�ʂ�ݒ�
	 * @param rotation ��]��
	*/
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;

		if (true == m_flagShadow) {
			m_shadowModel->SetRotation(m_rotation);
		}
	}

	/**
	 * @brief X���̉�]�ʂ�ݒ�
	 * @param rotation X���̉�]��
	*/
	void SetRotationX(const float rotation) {
		m_rotation.SetRotationX(rotation);
	}

	/**
	 * @brief �g�嗦��ݒ�
	 * @param scale �g�嗦
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;

		if (true == m_flagShadow) {
			m_shadowModel->SetScale(m_scale);
		}
	}

	/**
	 * @brief Hitbox�N���X�̈ʒu���X�V����ہA���̃N���X�ł������̂Ŏ����Ă���
	 * �i�֐����ς��邱�Ɓj
	 * @param hitbox Hitbox�N���X�̃C���X�^���X
	*/
	void SetTest(Hitbox& hitbox)
	{
		m_getHitbox = &hitbox;
	}

	/**
	 * @brief �X�E�F�[�̈ړ�����
	 * @param swayMove ���͏��
	*/
	void SetSwayMove(const Vector2& swayMoveController)
	{
		// ���E�����̊m�F
		// ����: �E
		if (EnSwayController::enNotMove < swayMoveController.x) {
			m_swayController[EnXY::x] = EnSwayController::enRight;
		}
		// ����: ��
		else if (EnSwayController::enNotMove > swayMoveController.x) {
			m_swayController[EnXY::x] = EnSwayController::enLeft;
		}
		// ����: �Ȃ�
		else {
			m_swayController[EnXY::x] = EnSwayController::enNotMove;
		}
		// �㉺�����̊m�F
		// ����: ��
		if (EnSwayController::enNotMove < swayMoveController.y) {
			m_swayController[EnXY::y] = EnSwayController::enUp;
		}
		// ����: ��
		else if (EnSwayController::enNotMove > swayMoveController.y) {
			m_swayController[EnXY::y] = EnSwayController::enDown;
		}
		// ����: �Ȃ�
		else {
			m_swayController[EnXY::y] = EnSwayController::enNotMove;
		}
	}


private: // enum
	/**
	 * @brief �X�E�F�[�̈ړ�����
	*/
	enum EnSwayController
	{
		enLeft = -1,
		enRight = 1,
		enUp = 1,
		enDown = -1,
		enNotMove = 0
	};

	/**
	 * @brief ���ݒu
	*/
	enum EnXY
	{
		x,
		y,
		MaxXY
	};

	int m_swayController[EnXY::MaxXY] = { EnSwayController::enNotMove }; // �X�E�F�[�̈ړ�����


private: // data member
    Model m_model;
	Shadow* m_shadowModel = nullptr; // �e�̕`�揈��
	std::unique_ptr<Skeleton> m_skeletonPointer; // �X�P���g���̃|�C���^�[
	std::unique_ptr<Animation> m_animationPointer; // �A�j���[�V�����̃|�C���^�[

    const char* m_tkmFilePath = nullptr; // tkm�t�@�C���̃t�@�C���p�X
	Light m_light;

	Vector3 m_position = g_vec3Zero;			// �ʒu
	Quaternion m_rotation = g_quatIdentity;		// ��]
	Vector3 m_scale = g_vec3One;				// �g��

	bool m_finishInit = false; // ���������I�������
	bool m_flagShadow = false; // �e���������邩

	Hitbox* m_getHitbox = nullptr; // �擾����Hitbox�N���X�̃C���X�^���X

	bool m_flagInitHitbox = false; // �����蔻���������������

	Vector2 m_swayMove = g_vec2Zero; // �X�E�F�[�̈ړ���
};