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
    bool Start2();
    void Update2();


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
    ModelRender* m_modelCharacter = nullptr; // �v���C���[�L�����N�^�[�̃��f��

    ModelRender* m_modelTest = nullptr;


    AnimationClip m_animationPlayer[AnimationMax];



    Vector3 m_position[100];



    // ����
    const int width = 50;
    const int height = 10;

    const int numHumanModel = width * height;

    Matrix* worldMatrixArray = nullptr;

    Model humanModel;

    Vector3* humanPos = nullptr;
    Quaternion qRot;

    StructuredBuffer worldMatrixSB;

    RenderContext& renderContext = g_graphicsEngine->GetRenderContext();




};