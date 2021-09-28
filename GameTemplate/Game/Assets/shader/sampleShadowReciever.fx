/*!
 * @brief 影が落とされるモデル用のシェーダー
 */

// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

// step-3 ライトビュープロジェクション行列の定数バッファーを定義
cbuffer ShadowCb:register(b1) {
    //ディレクションライトにアクセスするための変数
    float3 dirDirection; //ライトの方向
    float3 dirColor;	 //ライトのカラー

    //定数バッファーにポイントライト用の変数を追加
    float3 ptPosition;	 //ポイントライトの位置
    float3 ptColor;		 //ポイントライトのカラー
    float ptRange;		 //ポイントライトの影響範囲

    //スポットライトのデータにアクセスするための変数を追加する
    float3 spPosition;	//スポットライトの位置
    float3 spColor;		//スポットライトのカラー
    float spRange;		//スポットライトの射出範囲
    float3 spDirection;	//スポットライトの射出方向
    float spAngle;		//スポットライトの射出角度

    //視点のデータにアクセスするための変数を定数バッファーに追加する
    float3 eyePos;		 //視点の位置
    float3 ambientLight; //環境光

    //半球ライトのデータにアクセスするための変数を追加
    float3 groundColor;		//照り返しのライト
    float3 skyColor;		//天球ライト
    float3 groundNormal;	//地面の法線

    //影に使用
    float4x4 mLVP;
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
    float4 normal   : NORMAL;
    float3 tangent  : TANGENT;		//接ベクトル
    float3 biNormal : BINORMAL;		//従法線
    float2 uv 		: TEXCOORD0;	//UV座標。
    SSkinVSIn skinVert;				//スキン用のデータ。

    //float4 pos : POSITION;  // スクリーン空間でのピクセルの座標
    //float4 normal : NORMAL; // 法線
    //float2 uv : TEXCOORD0;  // UV座標
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
    float4 normal	    : NORMAL;
    float3 tangent  : TANGENT;		//接ベクトル
    float3 biNormal : BINORMAL;		//従法線
    float2 uv 			: TEXCOORD0;	//uv座標。
    float4 worldPos		: TEXCOORD1;
    //ピクセルシェーダーへの入力にカメラ空間の法線を追加する
    float3 normalInView : TEXCOORD2;	//カメラ空間の法線。

    //float4 pos : SV_POSITION;   // スクリーン空間でのピクセルの座標
    //float4 normal : NORMAL;     // 法線
    //float2 uv : TEXCOORD0;      // UV座標

    // step-4 ライトビュースクリーン空間での座標を追加
    float4 posInLVP : TEXCOORD3;	//ライトビュースクリーン空間でのピクセルの座標
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0);      // アルベドマップ
Texture2D<float4> g_toonMap : register(t4);  //トゥーンシェーダー用のテクスチャー
Texture2D<float4> g_shadowMap : register(t10);  // シャドウマップ
sampler g_sampler : register(s0);               // サンプラーステート

/// <summary>
/// 影が落とされる3Dモデル用の頂点シェーダー
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    float4x4 m;
    //if (hasSkin) {
    //    m = CalcSkinMatrix(vsIn.skinVert);
    //}
    //else {
    //    m = mWorld;
    //}
    m = mWorld;

    //ここから下を書くっぽい

    psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);

    //頂点法線をピクセルシェーダーに渡す
    psIn.normal = mul(m, vsIn.normal);
    psIn.normal = normalize(psIn.normal);

    //接ベクトル
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    //従法線
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));
    //UV
    psIn.uv = vsIn.uv;

    //カメラ空間の法線を求める
    psIn.normalInView = mul(mView, psIn.normal);

    // step-5 ライトビュースクリーン空間の座標を計算する
    psIn.posInLVP = mul(mLVP, psIn.worldPos);

    return psIn;



    // 通常の座標変換
    //SPSIn psIn;
    //float4 worldPos = mul(mWorld, vsIn.pos);
    //psIn.pos = mul(mView, worldPos);
    //psIn.pos = mul(mProj, psIn.pos);
    //psIn.uv = vsIn.uv;
    //psIn.normal = mul(mWorld, vsIn.normal);

    ////新規
    //psIn.worldPos = mul(mWorld, vsIn.pos);
    //psIn.pos = mul(mWorld, vsIn.pos);
    ////psIn.worldPos = psIn.pos;
    //psIn.pos = mul(mView, psIn.pos);
    //psIn.pos = mul(mProj, psIn.pos);
    ////頂点法線をピクセルシェーダーに渡す
    //psIn.normal = mul(mWorld, vsIn.normal);
    //psIn.normal = normalize(psIn.normal);
    ////UV
    //psIn.uv = vsIn.uv;

    // step-5 ライトビュースクリーン空間の座標を計算する
    //psIn.posInLVP = mul(mLVP, worldPos);

    //return psIn;
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
/// 影が落とされる3Dモデル用のピクセルシェーダー
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    //モデルのテクスチャから色をフェッチする
    float4 color = g_albedo.Sample(g_sampler, psIn.uv);
    
    //ハーフランバート拡散照明によるライティング計算
    float p = dot(psIn.normal, dirDirection);
    
    p *= -1.0f;
    
    // 内積の値を0以上の値にする
    p = max(0.0f, p);
    //float p = dot(psIn.normal * -1.0f, dirDirection.xyz);
    p = p * 0.5f + 0.5f;
    p = p * p;
    
    //計算結果よりトゥーンシェーダー用のテクスチャから色をフェッチする
    float4 Col = g_toonMap.Sample(g_sampler,float2(p,0.0f));
    
    //求まった色を乗算する
    //return color *= Col;
    color *= Col;






    //float4 color = g_albedo.Sample(g_sampler, psIn.uv);

    // step-6 ライトビュースクリーン空間からUV空間に座標変換
    //【注目】ライトビュースクリーン空間からUV座標空間に変換している。
    float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    // step-7 UV座標を使ってシャドウマップから影情報をサンプリング
    float3 shadowMap = 1.0f;
    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f
        ) {
        shadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV);
    }

    //テスト start
    //float4 Col = g_albedo.Sample(g_sampler, psIn.uv);

    //計算結果よりトゥーンシェーダー用のテクスチャから色をフェッチする
    //if (shadowMap.x == 0.5f) {
    //    Col = g_toonMap.Sample(g_sampler, float2(0.5f, 0.0f));
    //}

    //Col = g_toonMap.Sample(g_sampler, float2(0.2f, 0.0f));
    //テスト end

    // step-8 サンプリングした影情報をテクスチャカラーに乗算する
    //テクスチャカラーにシャドウマップからサンプリングした情報を掛け算する。
    //影が描き込まれていたら0.5になっているので、色味が落ちて影っぽくなる。
    color.xyz *= shadowMap;

    return color;
}
