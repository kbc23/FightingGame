#pragma once
#include "font_render.h"

/**
 * @brief プレイヤーに関するUIを管理するクラス
*/
class PlayerUI : public IGameObject
{
public:
    PlayerUI();
    ~PlayerUI();
    bool Start() override final;
    void Update() override final;



public: // Update data
    void UpdateHpUI(const int hp)
    {
        m_hpUI->SetText(hp);
    }



private: // data menber
    ////////////////////////////////////////////////////////////
    // クラスのインスタンス
    ////////////////////////////////////////////////////////////

    FontRender* m_hpUI = nullptr;

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

};