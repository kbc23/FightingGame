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

    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////
    /**
     * @brief 初期化
     * @param filePath モデルのファイルパス
     * @param initPos 初期位置
     * @param initRotAngle 初期向き
    */
    void Init(const Vector3& initPos, float initRotAngle);
    /**
     * @brief アニメーションデータの初期化
     * @return 初期化できたか
    */
    const bool AnimationInit();
    /**
     * @brief キャラクターデータの初期化
     * @param initPos 位置情報
     * @param initRot 回転情報
    */
    void CharacterInit(const Vector3& initPos, const float initRot);

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////
    /**
     * @brief 引数を元にステータスを更新する
     * @param addMoveAmount 更新する移動量
     * @param addRotAngle 更新する回転量
    */
    void AddStatus(Vector3& addMoveAmount, const Vector2& swayMove);
    /**
     * @brief キャラクターが相手の方向を見る処理
    */
    void PlayerViewingDirection();


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
    Quaternion& GetRotation()
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


public: // Set function
    /**
     * @brief 指定した攻撃アニメーションを再生
     * @param animation 攻撃アニメーション
    */
    void SetAttackAnimation(const int animation)
    {
        if (true == m_hp0) {
            return;
        }

        m_flagAttackAnimation = true;

        // 同じアニメーションを指定した際、最初から再生されるように通常時アニメーションを挟む
        m_modelCharacter->PlayAnimation(enIdle);
        //アニメーションの再生
        m_modelCharacter->PlayAnimation(animation);
    }

    void SetDamageAnimation(const int animation)
    {
        if (true == m_hp0) {
            return;
        }

        m_modelCharacter->PlayAnimation(enIdle);

        if (EnDamageStatus::enBodyDamage == animation) {
            m_modelCharacter->PlayAnimation(enDamageBody);
        }
        else if (EnDamageStatus::enHeadDamage == animation) {
            m_modelCharacter->PlayAnimation(enDamageHead);
        }
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

    void SetDefenceAnimation()
    {
        if (true == m_hp0) {
            return;
        }

        m_modelCharacter->PlayAnimation(enDefence);
    }

    void EndDefenceAnimation()
    {
        if (true == m_hp0) {
            return;
        }

        m_modelCharacter->PlayAnimation(enIdle);
    }


    void SetOtherActor(Actor& otherActor)
    {
        m_getOtherActor = &otherActor;
    }


    void KnockDownAnimation()
    {
        if (false == m_hp0) {
            return;
        }
        
        m_modelCharacter->PlayAnimation(enKnockedDown);
    }

    void HP_0()
    {
        m_hp0 = true;
    }



public: //enum
    /**
     * @brief アニメーションクリップ
    */
    enum AnimationEnum
    {
        enIdle,
        enJubHead_L,    // ジャブ
        enJubHead_R,
        enJubBody_L,
        enJubBody_R,
        enUppercut,
        enHook,
        enBodyBlow,
        enStraight,
        enCrouchingStart,
        enCrouching,
        enCrouchingEnd,
        enDefence,
        enDamageBody,
        enDamageHead,
        enKnockedDown,
        AnimationMax
    };

    /**
     * @brief しゃがみ関連のステータス
    */
    enum class EnCrouchingStatus
    {
        enNotCrouching,
        enStart,
        enCrouching,
        enEnd,
        MaxCrouchingStatus
    };

    EnCrouchingStatus m_crouchingStatus = EnCrouchingStatus::enNotCrouching;

    enum EnDamageStatus
    {
        enNotDamage,
        enBodyDamage,
        enHeadDamage,
        MaxEnDamageStatus
    };

    int m_damageStatus = EnDamageStatus::enNotDamage;


private: // data member
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // プレイヤーキャラクターのモデル
    ReadCSVFileCharacterData m_readCSVFileCharacterData;


    Actor* m_getOtherActor = nullptr;

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
    bool m_hp0 = false;
};