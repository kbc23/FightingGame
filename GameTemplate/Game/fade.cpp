#include "stdafx.h"
#include "fade.h"

#include "constant.h"

Fade::Fade()
{

}

Fade::~Fade()
{
    DeleteGO(m_spriteFade);
}

bool Fade::Start()
{
    //フェードに使用する画像の初期化
    m_spriteFade = NewGO<SpriteRender>(igo::EnPriority::fade);
    m_spriteFade->Init("Assets/Image/Fade.dds");
    m_spriteFade->SetMulColorW(m_amountOfFade);

    return true;
}

void Fade::Update()
{
    ChangeAmountOfFade();
}

void Fade::ChangeAmountOfFade()
{
    switch (m_statusFade) {
    case CurrentFadeState::wait:

        break;
    case CurrentFadeState::fadeOut:
        FadeOut();
        break;
    case CurrentFadeState::fadeIn:
        FadeIn();
        break;
    default:
        MessageBoxA(nullptr, "フェードにてエラーが発生しました。", "エラー", MB_OK);
        EmergencyStop();
        return;
    }
}

void Fade::FadeOut()
{
    //フェードアウトする
    m_amountOfFade += m_amountOfChangeFadeOut;

    //フェードアウトしきったか
    if (m_amountOfFade >= 1.0f) {
        m_amountOfFade = 1.0f;
        m_statusFade = CurrentFadeState::wait;
    }

    //画像のフェード量を変更
    m_spriteFade->SetMulColorW(m_amountOfFade);
}

void Fade::FadeIn()
{
    //フェードインする
    m_amountOfFade -= m_amountOfChangeFadeIn;
    
    //フェードインしきったか
    if (m_amountOfFade <= 0.0f) {
        m_amountOfFade = 0.0f;
        m_statusFade = CurrentFadeState::wait;
    }

    //画像のフェード量を変更
    m_spriteFade->SetMulColorW(m_amountOfFade);
}