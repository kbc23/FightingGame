#pragma once
#include "model_render.h"

class ModelRender;
class Actor;

class Audience : public IGameObject
{
public:
    Audience();
    ~Audience();
    bool Start() override final;
    void Update() override final;


private:
    void SetPosition(const int i1);

    void Step2();

    void a1(const int iii);



private:

    /**
 * @brief �A�j���[�V�����N���b�v
*/
    enum AnimationEnum
    {
        enRooting,
        AnimationMax
    };

private: //data member
    Actor* m_actor = nullptr; // �L�����N�^�[
    ModelRender* m_modelCharacter[4][25] = { nullptr }; // �v���C���[�L�����N�^�[�̃��f��

    ModelRender* m_modelTest = nullptr;


    AnimationClip m_animationPlayer[AnimationMax];



    Vector3 m_position[100];



};