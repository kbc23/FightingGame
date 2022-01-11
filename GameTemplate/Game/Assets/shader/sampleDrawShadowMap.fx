/*!
 * @brief シャドウマップ描画用のシェーダー
 */

// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

struct SSkinVSIn
{
    int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;  // モデルの頂点座標
    float4 normal : NORMAL; // 法線
    float2 uv : TEXCOORD0;  // UV座標
    SSkinVSIn skinVert;				//スキン用のデータ。
};
// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;   // スクリーン空間でのピクセルの座標
    float4 normal : NORMAL;     // 法線
    float2 uv : TEXCOORD0;      // uv座標
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0);      // アルベドマップ
Texture2D<float4> g_shadowMap : register(t10);  // シャドウマップ
sampler g_sampler : register(s0);               // サンプラーステート
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。


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
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    if (hasSkin) {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else {
        m = mWorld;
    }

    // step-8 シャドウマップ描画用の頂点シェーダーを実装
    //SPSIn psIn;
    psIn.pos = mul(m, vsIn.pos);
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(m, vsIn.normal);
    return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}

/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

/// <summary>
/// シャドウマップ描画用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // step-9 シャドウマップ描画用のピクセルシェーダーを実装
    return float4(0.5f, 0.5f, 0.5f, 1.0f);
}
