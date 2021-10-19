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
        float initRotAngle,
        Actor* pOtherActor
    );

    /**
     * @brief 
     * @param filePath モデルのファイルパス
     * @param playerNum プレイヤー番号
     * @param initPos 初期位置
     * @param initRot 初期向き
    */
    void DebugInit(const char* filePath, const int playerNum, const Vector3& initPos, const float initRot);

    /**
     * @brief 引数を元にステータスを更新する
     * @param addMoveAmount 更新する移動量
     * @param addRotAngle 更新する回転量
    */
    void AddStatus(Vector3& addMoveAmount, const float addRotAngle);



private:
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

    RigidBody* GetRigidBody()
    {
        return m_charaCon.GetRigidBody();
    }




private: // constant


private: // data menber
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // プレイヤーキャラクターのモデル
    Actor* m_otherActor = nullptr; // 対戦相手
    CharacterController m_charaCon; // キャラコン


    ////////////////////////////////////////////////////////////
    // キャラクターのステータス
    ////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f }; // 位置
    Quaternion m_rotation = g_quatIdentity; // 回転
    float m_rotY = 0.0f; // 回転量
    Vector3 m_scale = g_vec3One; // 拡大




    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か

};