/*!
 * @brief   ディズニーベースの物理ベースシェーダ
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

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;      // モデルの頂点座標
    float4 normal : NORMAL;     // 法線
    float4 tangent : TANGENT;
    float4 biNormal : BINORMAL;
    float2 uv : TEXCOORD0;      // UV座標
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       // スクリーン空間でのピクセルの座標
    float4 normal : NORMAL;         // 法線
    float4 tangent : TANGENT;       // 接ベクトル
    float4 biNormal : BINORMAL;     // 十ベクトル。
    float2 uv : TEXCOORD0;          // uv座標
    float4 worldPos : TEXCOORD1;    // ワールド空間でのピクセルの座標
    //step-7 カメラ空間でのZ値を記録する変数を追加。
    float3 depthInView : TEXCOORD2; // カメラ空間でのZ値。
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////
// サンプラーステート
sampler g_sampler : register(s0);


Texture2D<float4> g_albedo : register(t0);      // アルベドマップ

/// <summary>
/// 頂点シェーダー
/// <summary>
SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;
    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    //step-9 頂点シェーダーでカメラ空間でのZ値を設定する。
    psIn.depthInView = psIn.pos.z;
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(mWorld, vsIn.normal));
    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    psIn.uv = vsIn.uv;

    return psIn;
}

SPSIn VSSkinMain(SVSIn vsIn)
{
    SPSIn psIn;
    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    //step-9 頂点シェーダーでカメラ空間でのZ値を設定する。
    psIn.depthInView = psIn.pos.z;
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(mWorld, vsIn.normal));
    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    psIn.uv = vsIn.uv;

    return psIn;
}

/// <summary>
/// ピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn)  : SV_Target0
{
    return g_albedo.Sample( g_sampler, psIn.uv);
}
