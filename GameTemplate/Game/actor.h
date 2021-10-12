#pragma once
#include "model_render.h"

////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////

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
     * @param gamePad キャラクターの移動に使用するゲームパッド
     * @param filePath 
     * @param initPos 
     * @param initRotAngle 
     * @param pOtherActor 
    */
    void Init(
        GamePad& gamePad,
        const char* filePath,
        const Vector3& initPos,
        float initRotAngle,
        Actor* pOtherActor
    );

    void DebugInit(const char* filePath, const int playerNum, const Vector3& initPos, const float initRot);

    void AddStatus(Vector3& addMoveAmount, const float addRotAngle);



private:
    /**
     * @brief キャラクターモデルに必要なステータス情報を渡す
    */
    void SetModelStatus();

    void Move();



public: // Get function
    const Vector3& GetPosition()
    {
        return m_position;
    }

    const Quaternion& GetRotation()
    {
        return m_rotation;
    }




private: // constant


private: // data menber
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // プレイヤーキャラクターのモデル
    //GamePad* m_gamePad = nullptr;
    Actor* m_otherActor = nullptr; // 対戦相手
    CharacterController m_charaCon;


    ////////////////////////////////////////////////////////////
    // キャラクターのステータス
    ////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity; // 回転
    float m_rotY = 0.0f;
    Vector3 m_scale = g_vec3One; // 拡大




    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か

};