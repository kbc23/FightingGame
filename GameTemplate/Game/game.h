#pragma once
#include "model_render.h"
#include "shadow.h"

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Update() override final;


private: //data menber
    ModelRender* m_modelCharacter = nullptr;	//プレイヤーキャラクターのモデル
    ModelRender* m_model = nullptr;
    Shadow* m_shadowModelCharacter = nullptr;


    Vector3 m_position = { 0.0f,0.0f,0.0f };
};