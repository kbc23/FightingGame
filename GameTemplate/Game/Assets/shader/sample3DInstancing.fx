/*!
 * @brief   シンプルなインスタンス描画用のシェーダー。
 */

///////////////////////////////////////////////////
// 構造体
///////////////////////////////////////////////////

// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos 		: POSITION;		//モデルの頂点座標。
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;		//接ベクトル
    float3 biNormal : BINORMAL;		//従法線
    float2 uv 		: TEXCOORD0;	//UV座標。
    SSkinVSIn skinVert;				//スキン用のデータ。

    //float4 pos : POSITION;      // モデルの頂点座標
    //float2 uv : TEXCOORD0;      // UV座標

    //SSkinVSIn skinVert;				//スキン用のデータ。
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
    float3 normal	    : NORMAL;
    float3 tangent  : TANGENT;		//接ベクトル
    float3 biNormal : BINORMAL;		//従法線
    float2 uv 			: TEXCOORD0;	//uv座標。
    float4 worldPos		: TEXCOORD1;
    //ピクセルシェーダーへの入力にカメラ空間の法線を追加する
    float3 normalInView : TEXCOORD2;	//カメラ空間の法線。

    //float4 pos : SV_POSITION;       // スクリーン空間でのピクセルの座標    
    //float2 uv : TEXCOORD0;          // uv座標
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////
// サンプラーステート
sampler g_sampler : register(s0);
Texture2D<float4> g_albedo : register(t0);      // アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。

// step-7 ワールド行列の配列にアクセスするための変数を追加。
StructuredBuffer<float4x4> g_worldMatrixArray : register( t10 );

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
    [unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }

    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);

    return skinning;
}

/// <summary>
/// 頂点シェーダー
/// <summary>
SPSIn VSMain(
    SVSIn vsIn, 
    // step-8 頂点シェーダーの引数にインスタンスIDを追加。
    uint instanceId : SV_InstanceID
)
{
    SPSIn psIn;
    psIn.uv = vsIn.uv;

    // step-9 ワールド行列の配列とインスタンスIDを利用して座標変換。
    float4x4 m = g_worldMatrixArray[instanceId];
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);

    return psIn;
}

SPSIn VSSkinMain(
    SVSIn vsIn,
    // step-8 頂点シェーダーの引数にインスタンスIDを追加。
    uint instanceId : SV_InstanceID
)
{
    SPSIn psIn;
    psIn.uv = vsIn.uv;

    float4x4 SkinMatrix = CalcSkinMatrix(vsIn.skinVert);

    //psIn.pos = mul(t, vsIn.pos);
    //psIn.pos = mul(t, vsIn.pos);

    // step-9 ワールド行列の配列とインスタンスIDを利用して座標変換。
    float4x4 worldMatrix = g_worldMatrixArray[instanceId];

    float4x4 testPos = mul(worldMatrix, SkinMatrix);

    psIn.pos = mul(testPos, vsIn.pos);
    //psIn.worldPos = mul(worldMatrix, psIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);


    /*psIn.worldPos = psIn.pos;*/
    //psIn.pos = mul(mView, psIn.pos);
    //psIn.pos = mul(mProj, psIn.pos);

    //頂点法線をピクセルシェーダーに渡す
    psIn.normal = mul(SkinMatrix, vsIn.normal);
    psIn.normal = normalize(psIn.normal);

    //接ベクトル
    psIn.tangent = normalize(mul(SkinMatrix, vsIn.tangent));
    //従法線
    psIn.biNormal = normalize(mul(SkinMatrix, vsIn.biNormal));
    //UV
    psIn.uv = vsIn.uv;

    //カメラ空間の法線を求める
    psIn.normalInView = mul(mView, psIn.normal);





    return psIn;
}

/// <summary>
/// ピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn)  : SV_Target0
{
    return g_albedo.Sample( g_sampler, psIn.uv);
}
