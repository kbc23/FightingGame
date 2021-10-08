#pragma once
#include "model_render.h"

////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////

class Actor : public IGameObject
{
public:
    Actor();
    ~Actor();
    bool Start() override;
    void Update() override;

    void Init(
        GamePad& gamePad,
        const char* filePath,
        const Vector3& initPos,
        float initRotAngle,
        Actor* pOtherActor
    );

    void DebugInit(const char* filePath, const int playerNum, const Vector3& initPos);



private:
    /**
     * @brief プレイヤーの入力を受ける
    */
    void Controller();

    /**
     * @brief キャラクターモデルに必要なステータス情報を渡す
    */
    void SetModelStatus();




private: // constant
    const int m_MAX_HP = 1000; // プレイヤーの体力の最大値


private: // data menber
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    ModelRender* m_modelCharacter = { nullptr }; // プレイヤーキャラクターのモデル
    GamePad* m_gamePad = nullptr;
    Actor* m_otherActor = nullptr; // 対戦相手

    ////////////////////////////////////////////////////////////
    // キャラクターのステータス
    ////////////////////////////////////////////////////////////

    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity; // 回転
    float m_rotY = 0.0f;
    Vector3 m_scale = g_vec3One; // 拡大


    int m_hp = m_MAX_HP; // プレイヤーの体力

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagOperation = true; // 操作可能か

};