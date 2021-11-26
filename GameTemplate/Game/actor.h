#pragma once
#include "model_render.h"

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
     * @param pOtherActor 相手のモデル
    */
    void Init(
        const char* filePath,
        const Vector3& initPos,
        float initRotAngle
    );

    /**
     * @brief 
     * @param filePath モデルのファイルパス
     * @param playerNum プレイヤー番号
     * @param initPos 初期位置
     * @param initRot 初期向き
    */
    void DebugInit(const char* filePath, const Vector3& initPos, const float initRot);

    /**
     * @brief 引数を元にステータスを更新する
     * @param addMoveAmount 更新する移動量
     * @param addRotAngle 更新する回転量
    */
    void AddStatus(Vector3& addMoveAmount);

    /**
     * @brief キャラクターが移動方向に向く処理
     * @param addMoveAmount 更新する移動量
    */
    void Turn(Vector3& addMoveAmount);


private:
    void AttackAnimation();

    /**
     * @brief キャラクターモデルに必要なステータス情報を渡す
    */
    void SetModelStatus();



public: // Get function
    const Vector3& GetPosition()
    {
        return m_position;
    }

    const Quaternion& GetRotation()
    {
        return m_rotation;
    }

    const bool GetFlagAttackAnimation()
    {
        return m_flagAttackAnimation;
    }

    Bone& GetBone(const wchar_t boneName)
    {
        return m_modelCharacter->GetBone(boneName);
    }

    Skeleton& GetSkeleton()
    {
        return m_modelCharacter->GetSkeleton();
    }



public: // Set function
    void SetAttackAnimation(const int animation)
    {
        m_flagAttackAnimation = true;

        m_modelCharacter->PlayAnimation(idle);
        m_modelCharacter->PlayAnimation(animation); //アニメーションの再生
    }


    void SetTest(Hitbox& hitbox)
    {
        m_modelCharacter->SetTest(hitbox);
    }


public: //enum
    enum AnimationEnum
    {
        idle,
        jub,
        uppercut,
        hook,
        bodyBlow,
        straight,
        AnimationMax
    };


private: // constant


private: // data member
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // プレイヤーキャラクターのモデル

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