/*!
 * @brief	シンプルなモデルシェーダー。
 */


////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//ディレクションライト用のデータを受け取る定数バッファーを用意する
cbuffer DirectionLightCb : register(b1)
{
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
}

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn
{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;		//接ベクトル
	float3 biNormal : BINORMAL;		//従法線
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
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
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線マップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
Texture2D<float4> g_toonMap : register(t4);  //トゥーンシェーダー用のテクスチャー
sampler g_sampler : register(s0);	//サンプラステート。

///////////////////////////////////////////
// 関数宣言
///////////////////////////////////////////

//////////
// 反射光の計算
//////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);						//Lambert拡散反射光
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);		//Phong鏡面反射光
//////////
// ライト別の計算
//////////
float3 CalcLigFromDirectionLight(SPSIn psIn);	//ディレクションライトによる反射光
float3 CalcLigFromPointLight(SPSIn psIn);		//ポイントライトによる反射光
float3 CalcLigFromSpotLight(SPSIn psIn);		//スポットライトによる反射光
float CalcLigFromRimLight(SPSIn psIn);			//リムライトによる反射光
float3 CalcLigFromHemiLight(SPSIn psIn);		//半球ライトによる反射光


float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv);


////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

//法線マップから法線を得る
float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

	float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

	return newNormal;
}

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
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}

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
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
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
	return color *= Col;

	//ここから下は未使用

	psIn.normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

	//////////////////////////////
	// ディレクションライトによるLambert拡散反射光とPhong鏡面反射光の計算
	//////////////////////////////

	// ディレクションライトによるライティングを計算する
	float3 directionLig = CalcLigFromDirectionLight(psIn);

	//////////////////////////////
	// ポイントライトによるLambert拡散反射光とPhong鏡面反射光の計算
	//////////////////////////////

	// ポイントライトによるライティングを計算する
	float3 pointLig = CalcLigFromPointLight(psIn);

	//////////////////////////////
	// スポットライトによるLambert拡散反射光とPhong鏡面反射光の計算
	//////////////////////////////

	//スポットライトによるライティングを計算する
	float3 spotLig = CalcLigFromSpotLight(psIn);

	//////////////////////////////
	// リムライトの計算
	//////////////////////////////

	//リムライトによるライティングを計算する
	float rimPower = CalcLigFromRimLight(psIn);

	//////////////////////////////
	// 半球ライトの計算
	//////////////////////////////

	//半球ライトによるライティングを計算
	float3 hemiLight = CalcLigFromHemiLight(psIn);

	//////////////////////////////
	// 最後の仕上げ
	//////////////////////////////

	//ディレクションライト、ポイントライト、環境光を足し算
	float3 finalLig = directionLig + pointLig + ambientLight;

	//スポットライトの反射光を最終的な反射光に足し算する
	finalLig += spotLig;

	//最終的な反射光にリムライトの反射光を合算する
	//まずはリムライトのカラーを計算
	float3 rimColor = rimPower * dirColor;

	//最終的な反射光にリムの反射光を合算
	finalLig += rimColor;

	//半球ライトを最終的な反射光に加算する
	finalLig += hemiLight;

	//finalLig = directionLig + ambientLight;

	//最終計算
	float4 finalColor = g_albedo.Sample(g_sampler, psIn.uv);

	//最終出力カラーに光を乗算する
	finalColor.xyz *= finalLig;

	return finalColor;
}

/// <summary>
/// Lambert拡散反射光を計算する
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	// ピクセルの法線とライトの方向の内積を計算する
	float t = dot(normal, lightDirection);

	t *= -1.0f;

	// 内積の値を0以上の値にする
	t = max(0.0f, t);

	// 拡散反射光を計算する
	return lightColor * t;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
	// 反射ベクトルを求める
	float3 refVec = reflect(lightDirection, normal);
	refVec = normalize(refVec);
	// 光が当たったサーフェイスから視点に伸びるベクトルを求める
	float3 toEye = eyePos - worldPos;
	toEye = normalize(toEye);

	// 鏡面反射の強さを求める
	float t = dot(refVec, toEye);

	// 鏡面反射の強さを0以上の数値にする
	t = max(0.0f, t);

	// 鏡面反射の強さを絞る
	t = pow(t, 5.0f);

	// 鏡面反射光を求める
	return lightColor * t;
}

////////////////////////////////////////////////////////////
// ライト別の計算
////////////////////////////////////////////////////////////

/// <summary>
/// ディレクションライトによる反射光を計算
/// </summary
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromDirectionLight(SPSIn psIn)
{
	// ディレクションライトによるLambert拡散反射光を計算する
	float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, psIn.normal);

	//return float4(diffDirection,1.0f);

	// ディレクションライトによるPhong鏡面反射光を計算する
	float3 specDirection = CalcPhongSpecular(dirDirection, dirColor, psIn.worldPos, psIn.normal);

	return diffDirection + specDirection;
}

/// <summary>
/// ポイントライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromPointLight(SPSIn psIn)
{
	//サーフェイスに入射するポイントライトの光の向きを計算する
	float3 ligDir = psIn.worldPos - ptPosition;
	//正規化して大きさ１のベクトルにする。
	ligDir = normalize(ligDir);

	//減衰なしのLambert拡散反射光を計算する
	float3 diffPoint = CalcLambertDiffuse(
		ligDir,			//ライトの方向
		ptColor,		//ライトのカラー
		psIn.normal		//サーフェイスの法線
	);

	//減衰なしのPhong鏡面反射光を計算する
	float3 specPoint = CalcPhongSpecular(
		ligDir,			//ライトの方向
		ptColor,		//ライトのカラー
		psIn.worldPos,	//サーフェイスのワールド座標
		psIn.normal		//サーフェイスの法線
	);

	//距離による影響率を計算する
	//ポイントライトとの距離を計算する。
	float3 distance = length(psIn.worldPos - ptPosition);
	//影響率を距離に比例して小さくする。
	float affect = 1.0f - 1.0f / ptRange * distance;
	//影響力がマイナスにならないように補正をかける。
	if (affect < 0.0f) {
		affect = 0.0f;
	}
	//影響の仕方を指数関数的にする。
	affect = pow(affect, 3.0f);

	//拡散反射光と鏡面反射光に影響率を乗算して影響を弱める
	diffPoint *= affect;
	specPoint *= affect;

	return diffPoint + specPoint;
}

/// <summary>
/// スポットライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromSpotLight(SPSIn psIn)
{
	//サーフェイスに入射するスポットライトの光の向きを計算する
	//ピクセルの座標 - スポットライトの座標を計算
	float3 ligDir = psIn.worldPos - spPosition;
	//正規化
	ligDir = normalize(ligDir);

	//減衰なしのLambert拡散反射光を計算する
	float3 diffSpotLight = CalcLambertDiffuse(
		ligDir,		//ライトの方向
		spColor,	//ライトのカラー
		psIn.normal	//サーフェイスの法線
	);

	//減衰なしのPhong鏡面反射光を計算する
	float3 specSpotLight = CalcPhongSpecular(
		ligDir,
		spColor,
		psIn.worldPos,
		psIn.normal
	);

	//距離による影響率を計算する
	//スポットライトとの距離を計算する。
	float3 distance = length(psIn.worldPos - spPosition);

	//影響率は距離に比例して小さくなっていく。
	float affect = 1.0f - 1.0f / spRange * distance;
	//影響力がマイナスにならないように補正をかける。
	if (affect < 0.0f) {
		affect = 0.0f;
	}
	//影響の仕方を指数関数的にする。
	affect = pow(affect, 3.0f);

	//影響率を乗算して反射光を弱める
	diffSpotLight *= affect;
	specSpotLight *= affect;

	//入射光と射出方向の角度を求める
	//内積を求める。
	float angle = dot(ligDir, spDirection);
	//角度を求める。
	angle = acos(angle);

	//角度による影響率を求める
	//角度に比例して小さくなっていく影響率を計算
	affect = 1.0f - 1.0f / spAngle * angle;
	//影響力がマイナスにならないように補正をかける。
	if (affect < 0.0f) {
		affect = 0.0f;
	}
	//影響の仕方を指数関数的にする。
	affect = pow(affect, 0.5f);

	//角度による影響率を反射光に乗算して、影響を弱める
	diffSpotLight *= affect;
	specSpotLight *= affect;

	return diffSpotLight + specSpotLight;
}

/// <summary>
/// リムライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float CalcLigFromRimLight(SPSIn psIn)
{
	//サーフェイスの法線と光の入射方向に依存するリムの強さを求める
	float power1 = 1.0f - max(0.0f, dot(dirDirection, psIn.normal));

	//サーフェイスの法線と視線の方向に依存するリムの強さを求める
	float power2 = 1.0f - max(0.0f, psIn.normalInView.z * -1.0f);

	//最終的なリムの強さを求める
	float rimPower = power1 * power2;

	//pow()を使用して、強さの変化を指数関数的にする。
	return pow(rimPower, 1.3f);
}

/// <summary>
/// 半球ライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromHemiLight(SPSIn psIn)
{
	//半球ライトを計算する
	//サーフェイスの法線と地面の法線との内積を計算
	float t = dot(psIn.normal, groundNormal);

	//内積の結果を０～１の範囲に変換
	t = (t + 1.0f) / 2.0f;

	//地面色と天球色を補完率tで線形補完
	return lerp(groundColor, skyColor, t);
}