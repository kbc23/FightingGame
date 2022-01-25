#include "stdafx.h"
#include "audience.h"

#include "model_render.h"

Audience::Audience()
{

}

Audience::~Audience()
{

}

bool Audience::Start()
{
    m_animationPlayer[enRooting].Load("Assets/modelData/model/rooting.tka");
    //ループ再生をtrueにする
    m_animationPlayer[enRooting].SetLoopFlag(true);

    m_modelCharacter = NewGO<ModelRender>(igo::EnPriority::model);
    m_modelCharacter->InstancingInit(
        "Assets/modelData/model/model.tkm", 500, modelUpAxis::enModelUpAxisZ, m_animationPlayer, AnimationMax
    );

    m_modelCharacter->SetAllInstancScale({ 2.0f,2.0f,2.0f });


    m_modelCharacter->PlayAnimation(enRooting);

    return true;
}

void Audience::Update()
{

}