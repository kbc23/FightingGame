#include "stdafx.h"
#include "audience.h"

#include "actor.h"
#include "model_render.h"

Audience::Audience()
{

}

Audience::~Audience()
{

}

bool Audience::Start()
{
    //m_actor = NewGO<Actor>(igo::EnPriority::model);
    //m_actor->Init({ 0.0f,0.0f,0.0f }, 0.0f);
    //m_actor->InitAudience();

    for (int i = 0; i < 4; i++) {
        //SetPosition(i);


        //a1(i);
    }


    //m_modelTest = NewGO<ModelRender>(igo::EnPriority::model);
    //m_modelTest->SetFlagAudience();


    //m_modelTest->AudienceInit("Assets/modelData/model/model.tkm",
    //    false, true, modelUpAxis::enModelUpAxisZ,
    //    m_animationPlayer, AnimationEnum::AnimationMax
    //);

    //m_modelTest->SetScale({ 2.0f,2.0f,2.0f });

    //m_modelTest->PlayAnimation(enRooting);



    return true;
}

void Audience::Update()
{

}

void Audience::SetPosition(const int i1)
{
    m_position[i1] = { 0.0f ,100.0f,0.0f + (i1 * 50.0f) };
}

void Audience::Step2()
{

}

void Audience::a1(const int iii)
{
    for (int i = 0; i < 1; i++) {
        m_animationPlayer->Load("Assets/modelData/model/rooting.tka");
        m_animationPlayer[AnimationEnum::enRooting].SetLoopFlag(true);

        m_modelCharacter[iii][i] = NewGO<ModelRender>(igo::EnPriority::model);
        m_modelCharacter[iii][i]->SetFlagAudience();
        m_modelCharacter[iii][i]->Init("Assets/modelData/model/model.tkm",
            false, true, modelUpAxis::enModelUpAxisZ,
            m_animationPlayer, AnimationEnum::AnimationMax
        );

        m_modelCharacter[iii][i]->SetPosition({ 0.0f + (50.0f * i) ,100.0f,0.0f + (iii * 50.0f) });
        m_modelCharacter[iii][i]->SetScale({ 2.0f,2.0f,2.0f });

        m_modelCharacter[iii][i]->PlayAnimation(enRooting);
    }

}