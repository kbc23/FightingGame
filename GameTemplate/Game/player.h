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
     * @brief 初期化
     * @param gamePad キャラクターの移動に使用するゲームパッド
     * @param initPos 初期位置
     * @param initRotAngle 初期向き
     * @param pOtherPlayer 相手プレイヤー
    */
    void Init(
        GamePad& gamePad,
        const Vector3& initPos,
        float initRotAngle,
        int playerNum,
        Player* pOtherPlayer
    );

    /**
     * @brief Debugモード時の初期化
     * @param filePath モデルのファイルパス
     * @param playerNum プレイヤー番号
     * @param initPos 初期位置
     * @param initRot 初期向き
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
    const Vector3& Move();

    /**
     * @brief ダッシュ時のプレイヤーの移動量を計算する
     * @return 移動量
    */
    const Vector3& DashMove();

    ////////////////////////////////////////////////////////////
    // 攻撃関連
    ////////////////////////////////////////////////////////////

    void UpdateAttack();



    /**
     * @brief 攻撃情報を作成
    */
    const bool AttackCreate();

    /**
     * @brief 攻撃に関する毎フレームの処理
    */
    void AttackUpdate();

    /**
     * @brief 攻撃が当たった際の処理
    */
    void HitAttack();
    
    /**
     * @brief 攻撃情報をリセット
    */
    void FinishAttack();

    /**
     * @brief 攻撃時のアニメーションの開始
    */
    void AttackAnimationStart();

    /**
     * @brief 連続攻撃の確認
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
     * @brief 剛体を取得する関数。
     * @return 剛体
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
     * @brief ダメージの処理
     * @param damage ダメージ量
     * @return ダメージを与えられたか
    */
    const bool ReceiveDamage(const int damage//,
                            //const int defenseBreakPower,
                            //const int impactType
    )
    {
        // HPが０なら処理をしない
        if (true == m_flagHp_0) {
            return false;
        }

        // ダウン時の無敵の時、ダメージを与えない
        //if (true == m_downStatus.GetFlagInvincible()) {
        //    return false;
        //}

        //防御中
        //if (true == m_defenceData.GetFlagDefense()) {
        //    m_defenceData.DecreaseDefenseValue(defenseBreakPower);
        //    return true;
        //}

        // ダメージ処理
        m_hp -= damage;

        // のけぞりの処理
        //if (StAttackData::EnImpactType::enSqueeze == impactType) {
        //    m_squeezeStatus.StartSqueeze();
        //}
        //// ダウンの処理
        //else if (StAttackData::EnImpactType::enDown == impactType) {
        //    m_downStatus.StartDown();
        //}

        // HPの確認
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
        // HPが０以下の場合、HPを０に設定し、敗北した状態にする
        if (0 >= m_hp) {
            m_hp = 0;
            m_flagHp_0 = true;
        }
    }


private: // constant
    static const int m_MAX_HP = 1000; // プレイヤーの体力の最大値
    static const int m_MAX_MP = 4;


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
    AttackJudgment* m_attackJudgment = nullptr; // 攻撃判定
    PlayerUI* m_playerUI = nullptr; // プレイヤーに関するUI
    Hitbox* m_hitbox = nullptr; // プレイヤーの当たり判定、攻撃判定

    //////////////////////////////
    // FindGO
    //////////////////////////////

    PlayerCamera* m_findPlayerCamera = nullptr;
    GameData* m_findGameData = nullptr;

    ////////////////////////////////////////////////////////////
    // プレイヤーのステータス
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // プレイヤーの体力
    StAttackData m_attackData; // 攻撃関連のデータ
    StDashStatus m_dashStatus; // ダッシュ関連のデータ
    StSqueezeStatus m_squeezeStatus; // のけぞり関連のデータ
    StDownStatus m_downStatus; // ダウン関連のデータ
    StDefenseData m_defenceData; // 防御関連のデータ

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か
    bool m_flagHp_0 = false; // HPが０になったか
    bool m_flagGameEndStopOperation = false; // ゲームが終了して操作ができなくなっているか

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1; // 自分か相手のどちらかを区別する番号

    // Debug
    int m_defenseTime = 0;
};