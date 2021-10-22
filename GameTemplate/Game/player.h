#pragma once
#include "actor.h"

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
    void AttackCreate(const int attackType);

    /**
     * @brief �U���͈͂̈ʒu�����쐬
     * �ʒu���v���C���[�̑O���̂Ƃ��Ɏg�p
     * @return �U���͈͂̈ʒu
    */
    const Vector3& CreateAttackPosition();

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
    void ResetAttackData();

    ////////////////////////////////////////////////////////////
    // �_�b�V���֘A
    ////////////////////////////////////////////////////////////

    /**
     * @brief �_�b�V�����n�߂�Ƃ��̏���
    */
    void StartDash();

    /**
     * @brief �_�b�V���Ɋւ��閈�t���[���̏���
    */
    void DashUpdate();

    /**
     * @brief �_�b�V�����I������Ƃ��̏���
    */
    void EndDash();

    /**
     * @brief �_�b�V���̎c��񐔂̉񕜂̏���
    */
    void DashRecoveryTime();
    

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
    const bool ReceiveDamage(const int damage)
    {
        // HP���O�ɂȂ珈�������Ȃ�
        if (true == m_flagHp_0) {
            return false;
        }

        //�h�䒆
        if (true == m_flagDefense) {
            return false;
        }

        m_hp = m_hp - damage;

        // HP�̊m�F
        CheckHp();

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


private: // enum
    /**
     * @brief �U���̎��
    */
    enum EnAttackType
    {
        enNotAttacking, // �U�����Ă��Ȃ�
        enNormal, // �ʏ�U��
    };



private: // struct
    /**
     * @brief �U�����Ɋւ���f�[�^�̍\����
    */
    struct StAttackData
    {
        int power = 0; // �U����
        int time = 0; // �U������
        int timeLimit = 0; // �U���̐�������
        Vector3 Range = Vector3::Zero; //�U���͈�
        float positionUpY = 0.0f; // �㏸������Y���W�̗�
        bool flagAlreadyAttacked = false; // �U���������������Ă��邩
        bool flagAttackNow = false; // ���ݍU������
        int attackType = EnAttackType::enNotAttacking; // �U���̎��
    };

    StAttackData m_attackData;

    struct StDashStatus
    {
        // constant
        const int MAX_COUNT_DASH = 5; // �_�b�V�����Ă��鎞�Ԃ̏��
        const int MAX_RECOVERY_TIME = 50; // �c��񐔂̉񕜎��Ԃ̏��
        const int MAX_REMAINING_NUMBER_OF_TIMES = 3; // �_�b�V���̎c��񐔂̏��

        // data member
        bool flagDash = false; // �_�b�V������
        int countDash = 0; // �_�b�V�����Ă��鎞��
        int remainingNumberOfTimes = MAX_REMAINING_NUMBER_OF_TIMES; // �_�b�V���̎c���
        int countRecoveryTime = 0; // �c��񐔂̉񕜎���
    };

    StDashStatus m_dashStatus;


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