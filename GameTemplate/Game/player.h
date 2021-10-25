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
     * @brief 剛体を取得する関数。
     * @return 剛体
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

        //防御中
        if (true == m_flagDefense) {
            return false;
        }

        m_hp = m_hp - damage;

        // ノックバックの処理
        m_knockBackStatus.StartKnockBack();
        // ダウンの処理
        m_downStatus.StartDown();

        // HPの確認
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
        // HPが０以下の場合、HPを０に設定し、敗北した状態にする
        if (0 >= m_hp) {
            m_hp = 0;
            m_flagHp_0 = true;
        }
    }


private: // constant
    static const int m_MAX_HP = 1000; // プレイヤーの体力の最大値


private: // enum




private: // struct
    StAttackData m_attackData;

    StDashStatus m_dashStatus;

    StKnockBackStatus m_knockBackStatus;

    StDownStatus m_downStatus;


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

    //////////////////////////////
    // FindGO
    //////////////////////////////

    PlayerCamera* m_findPlayerCamera = nullptr;
    GameData* m_findGameData = nullptr;

    ////////////////////////////////////////////////////////////
    // プレイヤーのステータス
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // プレイヤーの体力

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か
    bool m_flagHp_0 = false; // HPが０になったか
    bool m_flagGameEndStopOperation = false; // ゲームが終了して操作ができなくなっているか
    bool m_flagDefense = false; // 防御中か

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1; // 自分か相手のどちらかを区別する番号
};