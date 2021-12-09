#pragma once
#include "model_render.h"
#include "read_CSV_file_character_data.h"

/**
 * @brief キャラクターのモデル関連の処理
*/
class Actor : public IGameObject
{
public:
    Actor();
    ~Actor();
    bool Start() override;
    void Update() override;

    /**
     * @brief 初期化
     * @param filePath モデルのファイルパス
     * @param initPos 初期位置
     * @param initRotAngle 初期向き
    */
    void Init(
        const char* filePath,
        const Vector3& initPos,
        float initRotAngle
    );

    /**
     * @brief 
     * @param filePath モデルのファイルパス
     * @param initPos 初期位置
     * @param initRot 初期向き
    */
    void DebugInit(const char* filePath, const Vector3& initPos, const float initRot);

    /**
     * @brief 引数を元にステータスを更新する
     * @param addMoveAmount 更新する移動量
     * @param addRotAngle 更新する回転量
    */
    void AddStatus(Vector3& addMoveAmount, const Vector2& swayMove);

    /**
     * @brief キャラクターが移動方向に向く処理
     * @param addMoveAmount 更新する移動量
    */
    void Turn(Vector3& addMoveAmount);


private:
    /**
     * @brief 攻撃関連のアニメーションの処理
    */
    void AttackAnimation();

    /**
     * @brief モデルにステータス情報を渡す
    */
    void SetModelStatus();

    /**
     * @brief しゃがみの毎フレームの処理
     * @param swayMoveY スウェーのY軸の移動量
    */
    void CrouchingUpdate(const float swayMoveY);

    /**
     * @brief しゃがみの開始
    */
    void CrouchingStart();

    /**
     * @brief しゃがみ中のあれこれ（毎フレーム）
    */
    void Crouching();

    /**
     * @brief しゃがみ起き上がり
    */
    void CrouchingEnd();


public: // Get function
    /**
     * @brief 位置を取得
     * @return 位置
    */
    const Vector3& GetPosition() const
    {
        return m_position;
    }

    /**
     * @brief 回転を取得
     * @return 回転
    */
    const Quaternion& GetRotation() const
    {
        return m_rotation;
    }

    /**
     * @brief 攻撃アニメーション中かを取得
     * @return 攻撃アニメーション中か
    */
    const bool GetFlagAttackAnimation() const
    {
        return m_flagAttackAnimation;
    }

    /**
     * @brief 指定した名前のボーンを取得
     * @param boneName ボーンの名前
     * @return 指定した名前のボーン
    */
    Bone& GetBone(const wchar_t boneName) const
    {
        return m_modelCharacter->GetBone(boneName);
    }

    /**
     * @brief スケルトンを取得
     * @return スケルトン
    */
    Skeleton& GetSkeleton() const
    {
        return m_modelCharacter->GetSkeleton();
    }

    Matrix& GetModelWorldMatrix()
    {
        m_modelCharacter->GetModelWorldMatrix();
    }


public: // Set function
    /**
     * @brief 指定した攻撃アニメーションを再生
     * @param animation 攻撃アニメーション
    */
    void SetAttackAnimation(const int animation)
    {
        m_flagAttackAnimation = true;

        // 同じアニメーションを指定した際、最初から再生されるように通常時アニメーションを挟む
        m_modelCharacter->PlayAnimation(enIdle);
        //アニメーションの再生
        m_modelCharacter->PlayAnimation(animation);
    }

    /**
     * @brief ModelRenderクラスでHitboxクラスの位置情報などの更新をしたいので渡す処理
     * （関数の名前をどうにかする）
     * @param hitbox Hitboxクラスのインスタンス
    */
    void SetTest(Hitbox& hitbox)
    {
        m_modelCharacter->SetTest(hitbox);
    }


public: //enum
    /**
     * @brief アニメーションクリップ
    */
    enum AnimationEnum
    {
        enIdle,
        enJub,
        enUppercut,
        enHook,
        enBodyBlow,
        enStraight,
        enCrouchingStart,
        enCrouching,
        enCrouchingEnd,
        AnimationMax
    };

    enum class EnCrouchingStatus
    {
        enNotCrouching,
        enStart,
        enCrouching,
        enEnd,
        MaxCrouchingStatus
    };

    EnCrouchingStatus m_crouchingStatus = EnCrouchingStatus::enNotCrouching;


private: // data member
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // プレイヤーキャラクターのモデル
    ReadCSVFileCharacterData m_readCSVFileCharacterData;

    ////////////////////////////////////////////////////////////
    // キャラクターのステータス
    ////////////////////////////////////////////////////////////

    Vector3 m_position = g_vec3Zero; // 位置
    Quaternion m_rotation = g_quatIdentity; // 回転
    float m_rotY = 0.0f; // 回転量
    Vector3 m_scale = g_vec3One; // 拡大

    AnimationClip m_animationPlayer[AnimationMax]; // アニメーション

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か
    bool m_flagAttackAnimation = false; // 攻撃時のアニメーションをしているか
};