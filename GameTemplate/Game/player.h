#pragma once
#include "actor.h"
#include "hitbox.h"
#include "st_player_status.h"

class PlayerCamera;
class PlayerUI;
class GameData;
class PlayerController;

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
    const Vector3 Move(const Vector3& moveAmountBeforeEditing);
    /**
     * @brief �_�b�V�����̃v���C���[�̈ړ��ʂ��v�Z����
     * @return �ړ���
    */
    const Vector3 DashMove();

    void Defence(const float moveY);


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
    Actor& GetActor()
    {
        return *m_actor;
    }

    void SetFlagDownWait(const bool flag)
    {
        m_flagDownWait = flag;
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
        if (m_otherPlayer->m_hitbox->GetDamageStatus() == EnDamageStatus::enBodyDamage) {
            m_hp -= damage;
        }
        else {
            m_hp -= damage * 2;
        }

        // HP�̊m�F
        CheckHp();

        // �_���[�W�A�j���[�V����
        m_actor->SetDamageAnimation(m_otherPlayer->m_hitbox->GetDamageStatus());

        return true;
    }

    void ZishouDamage()
    {
        // HP���O�Ȃ珈�������Ȃ�
        if (true == m_flagHp_0) {
            return;
        }

        m_hp = 0;

        // HP�̊m�F
        CheckHp();
    }


    /**
     * @brief �Q�[�����I���������̃t���O���Z�b�g
     * @param flag �Q�[�����I��������
    */
    void SetFlagGameEndStopOperation(const bool flag)
    {
        m_flagGameEndStopOperation = flag;
    }

    void DownUp()
    {
        // HP���O�Ȃ珈�������Ȃ�
        if (true == m_flagHp_0) {
            return;
        }

        if (false == GetFlagDown()) {
            return;
        }

        CountDownTime();

        if (true == m_gamePad->IsTrigger(enButtonA)) {
            ++m_downUpCount;
        }


        else if (10 <= m_downUpCount && 1 == m_countDown) {
            SetFlagDown(false);
            m_hp = m_HP_50_PERCENT;
            m_downUpCount = 0;
            m_actor->HP_0(false);
            m_actor->SetIdleAnimation();

            m_downCount = 0;
        }
        else if (20 <= m_downUpCount && 2 == m_countDown) {
            SetFlagDown(false);
            m_hp = m_HP_25_PERCENT;
            m_downUpCount = 0;
            m_actor->HP_0(false);
            m_actor->SetIdleAnimation();

            m_downCount = 0;
        }
    }

    void CountDownTime()
    {
        ++m_downCount;

        if (m_downCount >= 600) {
            m_flagHp_0 = true;
        }
    }

    int m_downUpCount = 0;
    int m_downCount = 0;


private: // Used in the Set function
    /**
     * @brief �̗͂��O�ɂȂ������̊m�F
    */
    void CheckHp()
    {
        // HP���O�ȉ��̏ꍇ�AHP���O�ɐݒ肵�A�s�k������Ԃɂ���
        if (0 >= m_hp) {
            m_hp = 0;
            SetFlagDown(true);
            m_otherPlayer->SetFlagDownWait(true);

            ++m_countDown;

            if (m_COUNT_DOWN_END == m_countDown) {
                m_flagHp_0 = true;
            }

            m_actor->HP_0(true);
        }
    }

private:
    enum EnDamageStatus
    {
        enNotDamage,
        enBodyDamage,
        enHeadDamage,
        MaxEnDamageStatus
    };


private: // constant
    static const int m_MAX_HP = 1000; // �v���C���[�̗̑͂̍ő�l
    static const int m_HP_50_PERCENT = 500;
    static const int m_HP_25_PERCENT = 250;

    static const int m_COUNT_DOWN_END = 3;


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
    PlayerController* m_playerController = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////

    PlayerCamera* m_findPlayerCamera = nullptr; // �v���C���[�̃J����
    GameData* m_findGameData = nullptr; // �Q�[���f�[�^

    ////////////////////////////////////////////////////////////
    // �v���C���[�̃X�e�[�^�X
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // �v���C���[�̗̑�
    StPlayerStatus m_playerStatus; // �v���C���[�̃X�e�[�^�X

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // ����\��
    bool m_flagHp_0 = false; // HP���O�ɂȂ�����
    bool m_flagGameEndStopOperation = false; // �Q�[�����I�����đ��삪�ł��Ȃ��Ȃ��Ă��邩

    bool m_flagDown = false;

    void SetFlagDown(const bool flag)
    {
        m_flagDown = flag;
    }

    const bool GetFlagDown()
    {
        return m_flagDown;
    }

    bool m_flagDownWait = false;
    int m_countDown = 0;

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1; // ����������̂ǂ��炩����ʂ���ԍ�
};