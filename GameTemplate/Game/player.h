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
    /**
     * @brief プレイヤーの入力を受ける
    */
    void Controller();

    /**
     * @brief プレイヤーの移動量を計算する
     * @return 移動量
    */
    const Vector3& Move();

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

    void AttackUpdate();

    /**
     * @brief 攻撃が当たった際の処理
    */
    void HitAttack();

    void DebugHitAttack(const float rotY);
    
    /**
     * @brief 攻撃情報をリセット
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
     * @brief 剛体を取得する関数。
     * @return 剛体
    */
    RigidBody* GetRigidBody()
    {
        return m_actor->GetRigidBody();
    }


private: // constant
    const int m_MAX_HP = 1000; // プレイヤーの体力の最大値


private: // enum
    /**
     * @brief 攻撃の種類
    */
    enum EnAttackType
    {
        notAttacking, // 攻撃していない
        normal, // 通常攻撃
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
        bool flagAlreadyAttacked = false; // 攻撃がもう当たっているか
        bool flagAttackNow = false; // 現在攻撃中か
        int attackType = EnAttackType::notAttacking; //攻撃の種類
    };

    StAttackData m_attackData;


private: // data menber
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

    ////////////////////////////////////////////////////////////
    // プレイヤーのステータス
    ////////////////////////////////////////////////////////////

    int m_hp = m_MAX_HP; // プレイヤーの体力

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_playerNum = -1; // 自分か相手のどちらかを区別する番号

    int m_attackTime = 0;
    bool m_flagAttack = false;
};