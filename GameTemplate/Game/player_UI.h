#pragma once
#include "font_render.h"

/**
 * @brief �v���C���[�Ɋւ���UI���Ǘ�����N���X
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
    // �N���X�̃C���X�^���X
    ////////////////////////////////////////////////////////////

    FontRender* m_hpUI = nullptr;

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

};