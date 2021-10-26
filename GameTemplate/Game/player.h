#pragma once
#include "actor.h"
#include "st_attack_data.h"
#include "st_dash_status.h"
#include "st_knock_back_status.h"
#include "st_down_status.h"

class PlayerCamera;
class AttackJudgment;
class PlayerUI;
class GameData;

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
    const Vector3& Move();

    /**
     * @brief �_�b�V�����̃v���C���[�̈ړ��ʂ��v�Z����
     * @return �ړ���
    */
    const Vector3& DashMove();

    ////////////////////////////////////////////////////////////
    // �U���֘A
    ////////////////////////////////////////////////////////////
    /**
     * @brief �U�������쐬
    */
    const bool AttackCreate();

    /**
     * @brief �U���Ɋւ��閈�t���[���̏���
    */
    void AttackUpdate();

    /**
     * @brief �U�������������ۂ̏���
    */
    void HitAttack();
    
    /**
     * @brief �U���������Z�b�g
    */
    void FinishAttack();


public: // Get function
    const Vector3& GetPosition()
    {
        return m_actor->GetPosition();
    }

    Actor& GetActor()
    {
        return *m_actor;
    }

    /**
     * @brief ���̂��擾����֐��B
     * @return ����
    */
    RigidBody* GetRigidBody()
    {
        return m_actor->GetRigidBody();
    }

    const bool CheckHp_0()
    {
        return m_flagHp_0;
    }


public: // Set function
    /**
     * @brief �_���[�W�̏���
     * @param damage �_���[�W��
     * @return �_���[�W��^����ꂽ��
    */
    const bool ReceiveDamage(const int damage, const int impactType)
    {
        // HP���O�Ȃ珈�������Ȃ�
        if (true == m_flagHp_0) {
            return false;
        }

        // �_�E�����̖��G�̎��A�_���[�W��^���Ȃ�
        if (true == m_downStatus.GetFlagInvincible()) {
            return false;
        }

        //�h�䒆
        if (true == m_flagDefense) {
            return false;
        }

        m_hp = m_hp - damage;

        // �m�b�N�o�b�N�̏���
        if (StAttackData::EnImpactType::enKnockBack == impactType) {
            m_knockBackStatus.StartKnockBack();
        }
        // �_�E���̏���
        else if (StAttackData::EnImpactType::enDown == impactType) {
            m_downStatus.StartDown();
        }

        // HP�̊m�F
        CheckHp();

        m_mp += 0.5;

        if (m_MAX_MP <= m_mp) {
            m_mp = m_MAX_MP;
        }

        return true;
    }

    void SetmFlagGameEndStopOperation(const bool flag)
    {
        m_flagGameEndStopOperation = flag;
    }


private: // Used in the Set function
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
    static const int m_MAX_MP = 4;


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
    AttackJudgment* m_attackJudgment = nullptr; // �U������
    PlayerUI* m_playerUI = nullptr; // �v���C���[�Ɋւ���UI

    //////////////////////////////
    // FindGO
    //////////////////////////////

    PlayerCamera* m_findPlayerCamera = nullptr;
    GameData* m_findGameData = nullptr;

    ////////////////////////////////////////////////////////////
    // �v���C���[�̃X�e�[�^�X
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // �v���C���[�̗̑�
    double m_mp = 0;
    StAttackData m_attackData; // �U���֘A�̃f�[�^
    StDashStatus m_dashStatus; // �_�b�V���֘A�̃f�[�^
    StKnockBackStatus m_knockBackStatus; // �m�b�N�o�b�N�֘A�̃f�[�^
    StDownStatus m_downStatus; // �_�E���֘A�̃f�[�^

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // ����\��
    bool m_flagHp_0 = false; // HP���O�ɂȂ�����
    bool m_flagGameEndStopOperation = false; // �Q�[�����I�����đ��삪�ł��Ȃ��Ȃ��Ă��邩
    bool m_flagDefense = false; // �h�䒆��

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1; // ����������̂ǂ��炩����ʂ���ԍ�
};