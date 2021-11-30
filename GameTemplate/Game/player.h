#pragma once
#include "actor.h"
#include "st_attack_data.h"
#include "st_dash_status.h"
#include "st_squeeze_status.h"
#include "st_down_status.h"
#include "st_defense_data.h"    
#include "hitbox.h"

class PlayerCamera;
class PlayerUI;
class GameData;
class AttackMoveBone;

class Player : public IGameObject
{
public:
    Player();
    ~Player();
    bool Start() override;
    void Update() override;

    /**
     * @brief ������
     * @param gamePad �L�����N�^�[�̈ړ��Ɏg�p����Q�[���p�b�h
     * @param initPos �����ʒu
     * @param initRotAngle ��������
     * @param playerNum �v���C���[�ԍ�
     * @param pOtherPlayer ����v���C���[
    */
    void Init(
        GamePad& gamePad,
        const Vector3& initPos,
        float initRotAngle,
        int playerNum,
        Player* pOtherPlayer
    );

    /**
     * @brief Debug���[�h���̏�����
     * @param filePath ���f���̃t�@�C���p�X
     * @param playerNum �v���C���[�ԍ�
     * @param initPos �����ʒu
     * @param initRot ��������
     * @param pOtherPlayer ����v���C���[
    */
    void DebugInit(
        const char* filePath,
        const int playerNum,
        const Vector3& initPos,
        const float initRot,
        Player* pOtherPlayer
    );


private:
    ////////////////////////////////////////////////////////////
    // �ړ�����
    ////////////////////////////////////////////////////////////

    /**
     * @brief �v���C���[�̓��͂��󂯂�
    */
    void Controller();

    /**
     * @brief �v���C���[�̈ړ��ʂ��v�Z����
     * @return �ړ���
    */
    const Vector3 Move();

    /**
     * @brief �_�b�V�����̃v���C���[�̈ړ��ʂ��v�Z����
     * @return �ړ���
    */
    const Vector3 DashMove();

    ////////////////////////////////////////////////////////////
    // �U���֘A
    ////////////////////////////////////////////////////////////

    /**
     * @brief �U���֘A�̖��t���[���̏���
    */
    void UpdateAttack();

    /**
     * @brief �U�������������ۂ̏���
    */
    void HitAttack();

    /**
     * @brief �U�����̃A�j���[�V�����̊J�n
    */
    void AttackAnimationStart();



public: // Get function
    /**
     * @brief �ʒu�����擾
     * @return �ʒu
    */
    const Vector3& GetPosition() const
    {
        return m_actor->GetPosition();
    }

    /**
     * @brief ��]�����擾
     * @return ��]
    */
    const Quaternion& GetRotation() const
    {
        return m_actor->GetRotation();
    }

    /**
     * @brief �����蔻��̃S�[�X�g�I�u�W�F�N�g���擾
     * @param bodyParts �g�̂̕���
     * @return �w�肵���g�̂̕��ʂ̓����蔻��̃S�[�X�g�I�u�W�F�N�g
    */
    btGhostObject& GetGhostObject(const int bodyParts) const
    {
        return m_hitbox->GetGhostObject(bodyParts);
    }

    /**
     * @brief �̗͂��O�ɂȂ�����
     * @return [true]: �̗͂��O, [false]: �̗͂��O�ł͂Ȃ�
    */
    const bool CheckHp_0() const
    {
        return m_flagHp_0;
    }

    StDefenseData& GetStDefenseData()
    {
        return m_defenceData;
    }

    Actor& GetActor()
    {
        return *m_actor;
    }


public: // Set function
    /**
     * @brief �_���[�W�̏���
     * @param damage �_���[�W��
     * @return �_���[�W��^����ꂽ��
    */
    const bool ReceiveDamage(const int damage)
    {
        // HP���O�Ȃ珈�������Ȃ�
        if (true == m_flagHp_0) {
            return false;
        }

        // �_���[�W����
        m_hp -= damage;

        // HP�̊m�F
        CheckHp();

        return true;
    }

    /**
     * @brief �Q�[�����I���������̃t���O���Z�b�g
     * @param flag �Q�[�����I��������
    */
    void SetFlagGameEndStopOperation(const bool flag)
    {
        m_flagGameEndStopOperation = flag;
    }


private: // Used in the Set function
    /**
     * @brief �̗͂��O�ɂȂ������̊m�F
    */
    void CheckHp()
    {
        // HP���O�ȉ��̏ꍇ�AHP���O�ɐݒ肵�A�s�k������Ԃɂ���
        if (0 >= m_hp) {
            m_hp = 0;
            m_flagHp_0 = true;
        }
    }


private: // constant
    static const int m_MAX_HP = 1000; // �v���C���[�̗̑͂̍ő�l


private: // data member
    ////////////////////////////////////////////////////////////
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    Actor* m_actor = nullptr; // �L�����N�^�[
    GamePad* m_gamePad = nullptr; // �Q�[���p�b�h
    Player* m_otherPlayer = nullptr; // �ΐ푊��
    PlayerUI* m_playerUI = nullptr; // �v���C���[�Ɋւ���UI
    Hitbox* m_hitbox = nullptr; // �v���C���[�̓����蔻��A�U������

    //////////////////////////////
    // FindGO
    //////////////////////////////

    PlayerCamera* m_findPlayerCamera = nullptr; // �v���C���[�̃J����
    GameData* m_findGameData = nullptr; // �Q�[���f�[�^

    ////////////////////////////////////////////////////////////
    // �v���C���[�̃X�e�[�^�X
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // �v���C���[�̗̑�
    StAttackData m_attackData; // �U���֘A�̃f�[�^
    StDashStatus m_dashStatus; // �_�b�V���֘A�̃f�[�^
    StSqueezeStatus m_squeezeStatus; // �̂�����֘A�̃f�[�^
    StDownStatus m_downStatus; // �_�E���֘A�̃f�[�^
    StDefenseData m_defenceData; // �h��֘A�̃f�[�^

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // ����\��
    bool m_flagHp_0 = false; // HP���O�ɂȂ�����
    bool m_flagGameEndStopOperation = false; // �Q�[�����I�����đ��삪�ł��Ȃ��Ȃ��Ă��邩

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1; // ����������̂ǂ��炩����ʂ���ԍ�
};