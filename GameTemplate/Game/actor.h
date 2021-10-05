#pragma once
#include "model_render.h"

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



private:
    /**
     * @brief プレイヤーの入力を受ける
    */
    void Controller();






private: //data menber
    ModelRender* m_modelCharacter = { nullptr }; //プレイヤーキャラクターのモデル
    GamePad* m_gamePad = nullptr;
    Actor* m_otherActor = nullptr; // 対戦相手


    Vector3 m_position = { 0.0f,0.0f,0.0f };
    Quaternion m_rotation = g_quatIdentity;	 //回転
    float m_rotY = 0.0f;



};