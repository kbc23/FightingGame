#include "stdafx.h"
#include "audience.h"

#include "actor.h"
#include "model_render.h"

Audience::Audience()
{
    // step-1 500体分のモデルの座標を計算する。
/*const int width = 50;
const int height = 10;*/
//const int numHumanModel = width * height;
    humanPos = new Vector3[numHumanModel];
    int humanNo = 0;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // 1000体のモデルが綺麗に並ぶように、座標を計算する。
            humanPos[humanNo].x = -2400.0f + 100.0f * x;
            humanPos[humanNo].y = -1250.0f + 250.0f * y;
            humanPos[humanNo].z = 0.0f;
            humanNo++;
        }
    }

    // step-2 500体分のワールド⾏列関係の各種バッファを確保。
    // まずは計算⽤のバッファをメインメモリ上に確保する。
    worldMatrixArray = new Matrix[numHumanModel];
    // 続いて、シェーダー側でワールド⾏列を使⽤するためのストラクチャードバッファをVRAM上に確保する。
    //StructuredBuffer worldMatrixSB;
    worldMatrixSB.Init(
        sizeof(Matrix), // 第⼀引数は１要素のサイズ。
        numHumanModel, // 第⼆引数は要素数。
        nullptr // 第三引数は初期値データ。初期値は指定しないので、今回はnullptr。
    );
}

Audience::~Audience()
{

}


namespace // 雑に作業する為に作られるクソネームスペース
{
    //const int width = 50;
    //const int height = 10;

    //const int numHumanModel = width * height;

    //Matrix* worldMatrixArray = nullptr;

    //Model humanModel;

    //Vector3* humanPos = nullptr;
    //Quaternion qRot;

    //StructuredBuffer worldMatrixSB;

    //auto& renderContext = g_graphicsEngine->GetRenderContext();
}

bool Audience::Start()
{


    // step-3 ⼈物のモデルを初期化。
    // モデルの初期化データを設定する。
    ModelInitData modelInitData;

    modelInitData.m_vsSkinEntryPointFunc = "VSMain";

    // tkmファイルのパスを指定。
    modelInitData.m_tkmFilePath = "Assets/modelData/model/model.tkm";
    // 使⽤するシェーダーファイルのパスを指定。
    modelInitData.m_fxFilePath = "Assets/shader/sample3DInstancing.fx";
    // 【注⽬】拡張SRVにストラクチャードバッファを渡す。
    modelInitData.m_expandShaderResoruceView[0] = &worldMatrixSB;
    // 設定したデータでモデルを初期化。
    //Model humanModel;
    humanModel.Init(modelInitData);

    //auto& renderContext = g_graphicsEngine->GetRenderContext();


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
    // 人間のモデルを回転させる。
    qRot.AddRotationY(0.01f);

    // step-4 ワールド⾏列を計算する。
    for (int i = 0; i < numHumanModel; i++) {
        // ワールド⾏列を計算する。
        worldMatrixArray[i] = humanModel.CalcWorldMatrix(humanPos[i], qRot,
            g_vec3One);
    }

    // step-5 ワールド⾏列の内容をグラフィックメモリにコピー。
    worldMatrixSB.Update(worldMatrixArray);

    // step-6 ⼈物のモデルをインスタンシング描画。
    humanModel.DrawInstancing(renderContext, numHumanModel);


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