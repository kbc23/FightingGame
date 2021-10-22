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
    void AttackCreate(const int attackType);

    /**
     * @brief 攻撃範囲の位置情報を作成
     * 位置がプレイヤーの前方のときに使用
     * @return 攻撃範囲の位置
    */
    const Vector3& CreateAttackPosition();

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
    void ResetAttackData();

    ////////////////////////////////////////////////////////////
    // ダッシュ関連
    ////////////////////////////////////////////////////////////

    /**
     * @brief ダッシュを始めるときの処理
    */
    void StartDash();

    /**
     * @brief ダッシュに関する毎フレームの処理
    */
    void DashUpdate();

    /**
     * @brief ダッシュが終わったときの処理
    */
    void EndDash();

    /**
     * @brief ダッシュの残り回数の回復の処理
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
        // HPが０になら処理をしない
        if (true == m_flagHp_0) {
            return false;
        }

        //防御中
        if (true == m_flagDefense) {
            return false;
        }

        m_hp = m_hp - damage;

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
    /**
     * @brief 攻撃の種類
    */
    enum EnAttackType
    {
        enNotAttacking, // 攻撃していない
        enNormal, // 通常攻撃
    };



private: // struct
    /**
     * @brief 攻撃情報に関するデータの構造体
    */
    struct StAttackData
    {
        int power = 0; // 攻撃力
        int time = 0; // 攻撃時間
        int timeLimit = 0; // 攻撃の制限時間
        Vector3 Range = Vector3::Zero; //攻撃範囲
        float positionUpY = 0.0f; // 上昇させるY座標の量
        bool flagAlreadyAttacked = false; // 攻撃がもう当たっているか
        bool flagAttackNow = false; // 現在攻撃中か
        int attackType = EnAttackType::enNotAttacking; // 攻撃の種類
    };

    StAttackData m_attackData;

    struct StDashStatus
    {
        // constant
        const int MAX_COUNT_DASH = 5; // ダッシュしている時間の上限
        const int MAX_RECOVERY_TIME = 50; // 残り回数の回復時間の上限
        const int MAX_REMAINING_NUMBER_OF_TIMES = 3; // ダッシュの残り回数の上限

        // data member
        bool flagDash = false; // ダッシュ中か
        int countDash = 0; // ダッシュしている時間
        int remainingNumberOfTimes = MAX_REMAINING_NUMBER_OF_TIMES; // ダッシュの残り回数
        int countRecoveryTime = 0; // 残り回数の回復時間
    };

    StDashStatus m_dashStatus;


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