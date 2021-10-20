#pragma once
#include "actor.h"

class PlayerCamera;
class AttackJudgment;
class PlayerUI;

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
    /**
     * @brief �v���C���[�̓��͂��󂯂�
    */
    void Controller();

    /**
     * @brief �v���C���[�̈ړ��ʂ��v�Z����
     * @return �ړ���
    */
    const Vector3& Move();

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

    void AttackUpdate();

    /**
     * @brief �U�������������ۂ̏���
    */
    void HitAttack();

    void DebugHitAttack(const float rotY);
    
    /**
     * @brief �U���������Z�b�g
    */
    void ResetAttackData();


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


private: // constant
    const int m_MAX_HP = 1000; // �v���C���[�̗̑͂̍ő�l


private: // enum
    /**
     * @brief �U���̎��
    */
    enum EnAttackType
    {
        notAttacking, // �U�����Ă��Ȃ�
        normal, // �ʏ�U��
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
        bool flagAlreadyAttacked = false; // �U���������������Ă��邩
        bool flagAttackNow = false; // ���ݍU������
        int attackType = EnAttackType::notAttacking; //�U���̎��
    };

    StAttackData m_attackData;


private: // data menber
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

    ////////////////////////////////////////////////////////////
    // �v���C���[�̃X�e�[�^�X
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // �v���C���[�̗̑�

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // ����\��

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1; // ����������̂ǂ��炩����ʂ���ԍ�

    int m_attackTime = 0;
    bool m_flagAttack = false;
};