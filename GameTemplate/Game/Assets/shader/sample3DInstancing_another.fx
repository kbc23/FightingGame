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
    float4 pos : POSITION;      // モデルの頂点座標
    float2 uv : TEXCOORD0;      // UV座標
    SSkinVSIn skinVert;				//スキン用のデータ。
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       // スクリーン空間でのピクセルの座標    
    float2 uv : TEXCOORD0;          // uv座標
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

SPSIn VSMainCore(
    SVSIn vsIn, 
    uniform bool hasSkin,
    // step-8 頂点シェーダーの引数にインスタンスIDを追加。
    uint instanceId : SV_InstanceID
)
{
    SPSIn psIn;
    float4x4 m;
    if (hasSkin) {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else {
        m = mWorld;
    }


    // step-9 ワールド行列の配列とインスタンスIDを利用して座標変換。
    m = g_worldMatrixArray[instanceId];
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);

    return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn, uint instanceId : SV_InstanceID)
{
    return VSMainCore(vsIn, false, instanceId);
}

/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn, uint instanceId : SV_InstanceID)
{
    return VSMainCore(vsIn, true, instanceId);
}

/// <summary>
/// ピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn)  : SV_Target0
{
    return g_albedo.Sample( g_sampler, psIn.uv);
}
