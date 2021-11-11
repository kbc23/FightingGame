#include "stdafx.h"
#include "player_camera.h"

PlayerCamera::PlayerCamera()
{

}

PlayerCamera::~PlayerCamera()
{

}

bool PlayerCamera::Start()
{
    return true;
}

void PlayerCamera::Update()
{
    UpdatePlayerCamera();
    UpdateOtherPlayerCamera();
}

void PlayerCamera::UpdatePlayerCamera()
{
    //�����_�̓G�l�~�[�̍��W�ɂ��܂�
    m_cameraTarget = m_enemyPosition;
    //�G�l�~�[����v���C���[�ɐL�т�x�N�g�������߂܂�
    Vector3 pos = m_playerPosition - m_enemyPosition;
    //�J�����̍����͈��ɂ������̂ŁAy������0�ɂ��܂�
    pos.y = 0.0f;
    //�x�N�g���𐳋K�����܂�
    pos.Normalize();
    //�X�J���[���|���܂�
    pos *= 200.0f;
    //�J�������ǂꂾ���v���C���[�̍��W��荂������ݒ肵�܂�
    pos.y = 150.0f;
    //�v���C���[�̍��W�ɋ��߂��x�N�g���𑫂��āA�J�����̍��W�Ƃ��܂�
    m_cameraPosition = m_playerPosition + pos;

    m_cameraTarget.y = 75.0f;

    g_camera3D->SetTarget(m_cameraTarget); // �J�����ɒ����_�̏���n��
    g_camera3D->SetPosition(m_cameraPosition); // �J�����Ɉʒu����n��

    //g_camera3D->MoveForward(-100.0f);
    g_camera3D->MoveRight(50.0f);
    g_camera3D->SetTarget(m_cameraTarget); // �J�����ɒ����_�̏���n��


    // �������牺�̓e�X�g�Œǉ�

    m_onelineCameraPosition = m_cameraPosition;
    return;

    m_playerRotation.AddRotationY(0.5f);

    Quaternion testRotation = g_quatIdentity;

    pos.y = 0;

    testRotation.SetRotation(g_vec3Zero, pos);

    //testRotation.AddRotationY(0.0f);

    m_cameraPosition.y = 0.0f;

    // �U���͈͂̃|�W�V��������L�����N�^�[�̃|�W�V�����̃x�N�g�����擾
    Vector3 toPos = m_cameraPosition - m_playerPosition;
    // �L�����N�^�[��Quaternion���g���ăx�N�g�����v���C���[�̑O�����ɉ�]������
    testRotation.Apply(toPos);

    // ��L�Ŏ擾������񂩂�A�U���͈͂𐶐�����|�W�V�������擾
    m_cameraPosition = m_playerPosition - toPos;

    //�J�������ǂꂾ���v���C���[�̍��W��荂������ݒ肵�܂�
    m_cameraPosition.y = 200.0f;

    g_camera3D->SetPosition(m_cameraPosition); // �J�����Ɉʒu����n��
}

void PlayerCamera::UpdateOtherPlayerCamera()
{
    //�����_�̓G�l�~�[�̍��W�ɂ��܂�
    m_otherCameraTarget = m_playerPosition;
    //�G�l�~�[����v���C���[�ɐL�т�x�N�g�������߂܂�
    Vector3 pos = m_enemyPosition - m_playerPosition;
    //�J�����̍����͈��ɂ������̂ŁAy������0�ɂ��܂�
    pos.y = 0.0f;
    //�x�N�g���𐳋K�����܂�
    pos.Normalize();
    //�X�J���[���|���܂�
    pos *= 400.0f;
    //�J�������ǂꂾ���v���C���[�̍��W��荂������ݒ肵�܂�
    pos.y = 200.0f;
    //�v���C���[�̍��W�ɋ��߂��x�N�g���𑫂��āA�J�����̍��W�Ƃ��܂�
    m_otherCameraPosition = m_enemyPosition + pos;

}