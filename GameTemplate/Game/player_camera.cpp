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
    //�����_�̓G�l�~�[�̍��W�ɂ��܂�
    m_target = m_enemyPosition;
    //�G�l�~�[����v���C���[�ɐL�т�x�N�g�������߂܂�
    Vector3 pos = m_playerPosition - m_enemyPosition;
    //�J�����̍����͈��ɂ������̂ŁAy������0�ɂ��܂�
    pos.y = 0.0f;
    //�x�N�g���𐳋K�����܂�
    pos.Normalize();
    //�X�J���[���|���܂�
    pos *= 500.0f;
    //�J�������ǂꂾ���v���C���[�̍��W��荂������ݒ肵�܂�
    pos.y = 300.0f;
    //�v���C���[�̍��W�ɋ��߂��x�N�g���𑫂��āA�J�����̍��W�Ƃ��܂�
    m_position = m_playerPosition + pos;

    g_camera3D->SetTarget(m_target); // �J�����ɒ����_�̏���n��
    g_camera3D->SetPosition(m_position); // �J�����Ɉʒu����n��
}