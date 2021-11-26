#pragma once
#include "actor.h"
#include "st_attack_data.h"
#include "st_dash_status.h"
#include "st_squeeze_status.h"
#include "st_down_status.h"
#include "st_defense_data.h"    
#include "hitbox.h"

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

    void UpdateAttack();



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

    /**
     * @brief �U�����̃A�j���[�V�����̊J�n
    */
    void AttackAnimationStart();

    /**
     * @brief �A���U���̊m�F
    */
    //const bool CheckContinuousAttack(const int attackType);


public: // Get function
    const Vector3& GetPosition()
    {
        return m_actor->GetPosition();
    }

    const Quaternion& GetRotation()
    {
        return m_actor->GetRotation();
    }

    Actor& GetActor()
    {
        return *m_actor;
    }

    btGhostObject& GetGhostObject(const int bodyParts)
    {
        return m_hitbox->GetGhostObject(bodyParts);
    }

    /**
     * @brief ���̂��擾����֐��B
     * @return ����
    */
    //RigidBody* GetRigidBody()
    //{
    //    return m_actor->GetRigidBody();
    //}

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
    const bool ReceiveDamage(const int damage//,
                            //const int defenseBreakPower,
                            //const int impactType
    )
    {
        // HP���O�Ȃ珈�������Ȃ�
        if (true == m_flagHp_0) {
            return false;
        }

        // �_�E�����̖��G�̎��A�_���[�W��^���Ȃ�
        //if (true == m_downStatus.GetFlagInvincible()) {
        //    return false;
        //}

        //�h�䒆
        //if (true == m_defenceData.GetFlagDefense()) {
        //    m_defenceData.DecreaseDefenseValue(defenseBreakPower);
        //    return true;
        //}

        // �_���[�W����
        m_hp -= damage;

        // �̂�����̏���
        //if (StAttackData::EnImpactType::enSqueeze == impactType) {
        //    m_squeezeStatus.StartSqueeze();
        //}
        //// �_�E���̏���
        //else if (StAttackData::EnImpactType::enDown == impactType) {
        //    m_downStatus.StartDown();
        //}

        // HP�̊m�F
        CheckHp();

        return true;
    }

    void SetFlagGameEndStopOperation(const bool flag)
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
    Hitbox* m_hitbox = nullptr; // �v���C���[�̓����蔻��A�U������

    //////////////////////////////
    // FindGO
    //////////////////////////////

    PlayerCamera* m_findPlayerCamera = nullptr;
    GameData* m_findGameData = nullptr;

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

    // Debug
    int m_defenseTime = 0;
};