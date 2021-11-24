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

    g_camera3D->SetPosition(m_cameraPosition); // �J�����Ɉʒu�̏���n��
    //g_camera3D->MoveRight(100.0f); // �J�����̈ʒu���E�ɏ������炷
    g_camera3D->SetTarget(m_cameraTarget); // �J�����ɒ����_�̏���n��
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