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
     * @brief 初期化
     * @param gamePad キャラクターの移動に使用するゲームパッド
     * @param initPos 初期位置
     * @param initRotAngle 初期向き
     * @param playerNum プレイヤー番号
     * @param pOtherPlayer 相手プレイヤー
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
    // 移動処理
    ////////////////////////////////////////////////////////////
    /**
     * @brief プレイヤーの入力を受ける
    */
    void Controller();
    /**
     * @brief プレイヤーの移動量を計算する
     * @return 移動量
    */
    const Vector3 Move(const Vector3& moveAmountBeforeEditing);
    /**
     * @brief ダッシュ時のプレイヤーの移動量を計算する
     * @return 移動量
    */
    const Vector3 DashMove();

    void Defence(const float moveY);


    ////////////////////////////////////////////////////////////
    // 攻撃関連
    ////////////////////////////////////////////////////////////
    /**
     * @brief 攻撃関連の毎フレームの処理
    */
    void UpdateAttack();
    /**
     * @brief 攻撃が当たった際の処理
    */
    void HitAttack();
    /**
     * @brief 攻撃時のアニメーションの開始
    */
    void AttackAnimationStart();



public: // Get function
    /**
     * @brief 位置情報を取得
     * @return 位置
    */
    const Vector3& GetPosition() const
    {
        return m_actor->GetPosition();
    }
    /**
     * @brief 回転情報を取得
     * @return 回転
    */
    const Quaternion& GetRotation() const
    {
        return m_actor->GetRotation();
    }
    /**
     * @brief 当たり判定のゴーストオブジェクトを取得
     * @param bodyParts 身体の部位
     * @return 指定した身体の部位の当たり判定のゴーストオブジェクト
    */
    btGhostObject& GetGhostObject(const int bodyParts) const
    {
        return m_hitbox->GetGhostObject(bodyParts);
    }
    /**
     * @brief 体力が０になったか
     * @return [true]: 体力が０, [false]: 体力が０ではない
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
     * @brief ダメージの処理
     * @param damage ダメージ量
     * @return ダメージを与えられたか
    */
    const bool ReceiveDamage(const int damage)
    {
        // HPが０なら処理をしない
        if (true == m_flagHp_0) {
            return false;
        }

        // ダメージ処理
        if (m_otherPlayer->m_hitbox->GetDamageStatus() == EnDamageStatus::enBodyDamage) {
            m_hp -= damage;
        }
        else {
            m_hp -= damage * 2;
        }

        // HPの確認
        CheckHp();

        // ダメージアニメーション
        m_actor->SetDamageAnimation(m_otherPlayer->m_hitbox->GetDamageStatus());

        return true;
    }

    void ZishouDamage()
    {
        // HPが０なら処理をしない
        if (true == m_flagHp_0) {
            return;
        }

        m_hp = 0;

        // HPの確認
        CheckHp();
    }


    /**
     * @brief ゲームが終了したかのフラグをセット
     * @param flag ゲームが終了したか
    */
    void SetFlagGameEndStopOperation(const bool flag)
    {
        m_flagGameEndStopOperation = flag;
    }

    void DownUp()
    {
        // HPが０なら処理をしない
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
     * @brief 体力が０になったかの確認
    */
    void CheckHp()
    {
        // HPが０以下の場合、HPを０に設定し、敗北した状態にする
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
    static const int m_MAX_HP = 1000; // プレイヤーの体力の最大値
    static const int m_HP_50_PERCENT = 500;
    static const int m_HP_25_PERCENT = 250;

    static const int m_COUNT_DOWN_END = 3;


private: // data member
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    Actor* m_actor = nullptr; // キャラクター
    GamePad* m_gamePad = nullptr; // ゲームパッド
    Player* m_otherPlayer = nullptr; // 対戦相手
    PlayerUI* m_playerUI = nullptr; // プレイヤーに関するUI
    Hitbox* m_hitbox = nullptr; // プレイヤーの当たり判定、攻撃判定
    PlayerController* m_playerController = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////

    PlayerCamera* m_findPlayerCamera = nullptr; // プレイヤーのカメラ
    GameData* m_findGameData = nullptr; // ゲームデータ

    ////////////////////////////////////////////////////////////
    // プレイヤーのステータス
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // プレイヤーの体力
    StPlayerStatus m_playerStatus; // プレイヤーのステータス

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か
    bool m_flagHp_0 = false; // HPが０になったか
    bool m_flagGameEndStopOperation = false; // ゲームが終了して操作ができなくなっているか

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
    // その他
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1; // 自分か相手のどちらかを区別する番号
};