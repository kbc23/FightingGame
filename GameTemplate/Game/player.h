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

    /**
     * @brief Debugモード時の初期化
     * @param filePath モデルのファイルパス
     * @param playerNum プレイヤー番号
     * @param initPos 初期位置
     * @param initRot 初期向き
     * @param pOtherPlayer 相手プレイヤー
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
    const Vector3 Move();

    /**
     * @brief ダッシュ時のプレイヤーの移動量を計算する
     * @return 移動量
    */
    const Vector3 DashMove();

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
        m_hp -= damage;

        // HPの確認
        CheckHp();

        return true;
    }

    /**
     * @brief ゲームが終了したかのフラグをセット
     * @param flag ゲームが終了したか
    */
    void SetFlagGameEndStopOperation(const bool flag)
    {
        m_flagGameEndStopOperation = flag;
    }


private: // Used in the Set function
    /**
     * @brief 体力が０になったかの確認
    */
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

    //////////////////////////////
    // FindGO
    //////////////////////////////

    PlayerCamera* m_findPlayerCamera = nullptr; // プレイヤーのカメラ
    GameData* m_findGameData = nullptr; // ゲームデータ

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
};