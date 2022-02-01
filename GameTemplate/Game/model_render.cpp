#include "stdafx.h"
#include "model_render.h"

#include "shadow_map.h"
#include "shadow_light_camera.h"



namespace // constant
{
	const float SWAY_MOVE = 0.2f; // １フレームのスウェーの移動量
}


void Light::InitLight()
{
	// ライトの設定をする。
	// ディレクションライトを初期化する
	InitDirectionLight();

	// ポイントライトを初期化する
	//InitPointLight();

	// スポットライトを初期化する
	//InitSpotLight();

	// 環境光を初期化する
	InitAmbientLight();

	// 半球ライトを初期化する
	//InitHemiLight();
}

void Light::InitDirectionLight()
{
	// ライトは斜め上から当たっている。
	dirDirection.x = 1.0f;
	dirDirection.y = -1.0f;
	dirDirection.z = -1.0f;
	// 正規化する。
	dirDirection.Normalize();
	// ライトのカラーの設定（ライトの強さ）
	dirColor.x = 1.5f;
	dirColor.y = 1.5f;
	dirColor.z = 1.5f;

	// 視点の位置を設定
	eyePos = g_camera3D->GetPosition();
}

void Light::InitPointLight()
{
	// ポイントライトの初期座標を設定する
	ptPosition.x = 0.0f;
	ptPosition.y = 50.0f;
	ptPosition.z = 50.0f;

	// ポイントライトの初期カラーを設定する
	ptColor.x = 15.0f;
	ptColor.y = 0.0f;
	ptColor.z = 240.0f;

	// ポイントライトの影響範囲を設定する
	ptRange = 100.0f;
}

void Light::InitSpotLight()
{
	// 初期座標
	spPosition.x = 0.0f;
	spPosition.y = 50.0f;
	spPosition.z = -150.0f;

	// ライトのカラー
	spColor.x = 200.0f;
	spColor.y = 10.0f;
	spColor.z = 10.0f;
	// 初期方向は斜め下にする。
	spDirection.x = 1.0f;
	spDirection.y = -1.0f;
	spDirection.z = 1.0f;
	// 正規化
	spDirection.Normalize();
	// 射出範囲は300
	spRange = 300.0f;
	// 射出角度は25度
	spAngle = Math::DegToRad(25.0f);
}

void Light::InitAmbientLight()
{
	// 環境光
	ambientLight.x = 0.05f;
	ambientLight.y = 0.05f;
	ambientLight.z = 0.05f;
}

void Light::InitHemiLight()
{
	// 地面色、天球色、地面の法線のデータを設定する
	groundColor.x = 0.7f;
	groundColor.y = 0.5f;
	groundColor.z = 0.3f;

	// 天球色を設定
	skyColor.x = 0.15f;
	skyColor.y = 0.7f;
	skyColor.z = 0.95f;

	// 地面の法線を設定
	groundNormal.x = 0.0f;
	groundNormal.y = 1.0f;
	groundNormal.z = 0.0f;
}




ModelRender::ModelRender()
{

}

ModelRender::~ModelRender()
{
	DeleteGO(m_shadowModel);

	delete[] m_instancing.m_position;
	delete[] m_instancing.m_rotation;
	delete[] m_instancing.m_scale;
}

bool ModelRender::Start()
{
	return true;
}

////////////////////////////////////////////////////////////
// 初期化
////////////////////////////////////////////////////////////

void ModelRender::Init(const char* filePath,
	bool flagShadowReceiver, bool flagShadow,
	modelUpAxis::EnModelUpAxis modelUpAxis,
	AnimationClip* animationClip,
	int maxAnimationClipNum
)
{
	// モデルの描画方法を設定
	SetRenderType(EnRenderType::enNormal);

	// tkmファイルのファイルパスを保持
	m_tkmFilePath = filePath;

	// スケルトンのデータの読み込み
	InitSkeleton(filePath);
	// モデルの初期化
	InitModel(filePath, flagShadowReceiver, flagShadow, modelUpAxis);
	// アニメーションを初期化
	InitAnimation(animationClip, maxAnimationClipNum);

	// 初期化完了
	m_finishInit = true;
}

void ModelRender::InstancingInit(
	const char* filePath,
	const int modelMaxNum,
	modelUpAxis::EnModelUpAxis modelUpAxis,
	AnimationClip* animationClip,
	int maxAnimationClipNum
)
{
	// モデルの描画方法を設定
	SetRenderType(EnRenderType::enInstancing);

	// tkmファイルのファイルパスを保持
	m_tkmFilePath = filePath;

	// スケルトンのデータの読み込み
	InitSkeleton(filePath);
	// モデルの初期化
	InitInstancingModel(filePath, modelMaxNum, modelUpAxis);
	// アニメーションを初期化
	InitAnimation(animationClip, maxAnimationClipNum);

	// 初期化完了
	m_finishInit = true;
}

void ModelRender::OffScreenInit(
	const char* filePath,
	const int modelMaxNum,
	modelUpAxis::EnModelUpAxis modelUpAxis,
	AnimationClip* animationClip,
	int maxAnimationClipNum
)
{
	// モデルの描画方法を設定
	SetRenderType(EnRenderType::enOffScreen);

	// tkmファイルのファイルパスを保持
	m_tkmFilePath = filePath;

	// スケルトンのデータの読み込み
	InitSkeleton(filePath);
	// モデルの初期化
	InitOffScreenModel(filePath, modelMaxNum, modelUpAxis);
	// アニメーションを初期化
	InitAnimation(animationClip, maxAnimationClipNum);

	// 初期化完了
	m_finishInit = true;
}

bool ModelRender::InitSkeleton(const char* filePath)
{
	// tkmファイルをtksファイルに変換
	std::string skeletonFilePath = filePath;
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");

	// スケルトンのリソースを確保
	m_skeletonPointer.reset(new Skeleton);
	// スケルトンのデータの読み込み
	bool isInited = m_skeletonPointer->Init(skeletonFilePath.c_str());

	// 初期化に成功したか
	// 成功
	if (isInited) {
		return true;
	}
	// 失敗
	else {
		// スケルトンのリソースを解放
		m_skeletonPointer.reset();
		return false;
	}
}

void ModelRender::InitAnimation(AnimationClip* animationClip, int maxAnimationClipNum)
{
	// アニメ―ションクリップを登録しているか
	if (animationClip == nullptr) {
		return;
	}

	// アニメーションのリソースを確保
	m_animationPointer.reset(new Animation);
	// アニメーションを初期化
	m_animationPointer->Init(
		*m_skeletonPointer,
		animationClip,
		maxAnimationClipNum
	);
}

void ModelRender::InitModel(const char* filePath,
	bool flagShadowReceiver, bool flagShadow,
	modelUpAxis::EnModelUpAxis modelUpAxis
)
{
	// ライトの設定をする。
	m_light.InitLight();

	m_light.shadowCamera = ShadowLightCamera::GetInstance()->GetShadowLightCamera().GetViewProjectionMatrix();

	// 3Dモデルをロードするための情報を設定する
	// モデルの初期化するための情報を設定
	ModelInitData modelInitData;
	// tkmファイルのファイルパスを設定
	modelInitData.m_tkmFilePath = filePath;

	// 使用するシェーダーファイルパスを設定

	// 影を受けるか
	if (false == flagShadowReceiver) {
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";

		// ライトの情報を定数バッファとしてディスクリプタヒープに
		// 登録するためにモデルの初期化情報として渡す。
		modelInitData.m_expandConstantBuffer[0] = &m_light;
		modelInitData.m_expandConstantBufferSize[0] = sizeof(m_light);
	}
	else {
		modelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";
		// シャドウマップを拡張SRVに設定する。
		modelInitData.m_expandShaderResoruceView[0] = 
			&ShadowMap::GetInstance()->GetShadowMap().GetRenderTargetTexture();

		// ライトビュープロジェクション行列を拡張定数バッファに設定する。
		modelInitData.m_expandConstantBuffer[0] = &m_light;
		modelInitData.m_expandConstantBufferSize[0] = sizeof(m_light);
	}
	// 影を生成するか
	if (true == flagShadow) {
		m_shadowModel = NewGO<Shadow>(0);

		m_shadowModel->Init(filePath, *m_skeletonPointer);

		m_flagShadow = true;
	}

	// スケルトンを指定する。
	if (m_skeletonPointer) {	// スケルトンが初期化されていたら
		modelInitData.m_skeleton = m_skeletonPointer.get();
	}
	// モデルの上方向を指定
	modelInitData.m_modelUpAxis = modelUpAxis;

	// 初期化情報を使ってモデル表示処理を初期化する
	m_model.Init(modelInitData);
}

void ModelRender::InitInstancingModel(
	const char* filePath,
	const int modelMaxNum,
	modelUpAxis::EnModelUpAxis modelUpAxis
)
{
	// インスタンスの数をセット
	m_instancing.m_instanceNum = modelMaxNum;

	// モデルの座標を初期化
	m_instancing.m_position = new Vector3[m_instancing.m_instanceNum];
	m_instancing.m_rotation = new Quaternion[m_instancing.m_instanceNum];
	m_instancing.m_scale = new Vector3[m_instancing.m_instanceNum];

	// モデルの座標、回転、拡大率を初期化
	for (int instanceNum = 0; instanceNum < m_instancing.m_instanceNum; instanceNum++) {
		// インスタンス全ての座標を原点、回転をデフォルト、拡大率を１倍に設定
		m_instancing.m_position[instanceNum] = Vector3::Zero;
		m_instancing.m_rotation[instanceNum] = Quaternion::Identity;
		m_instancing.m_scale[instanceNum] = Vector3::One;
	}

	// インスタンシング描画のワールド⾏列関係の各種バッファを確保
	// まずは計算⽤のバッファをメインメモリ上に確保する
	m_instancing.m_worldMatrixArray = new Matrix[m_instancing.m_instanceNum];
	// 続いて、シェーダー側でワールド⾏列を使⽤するためのストラクチャードバッファをVRAM上に確保する
	m_instancing.m_worldMatrixSB.Init(
		sizeof(Matrix), // 第⼀引数は１要素のサイズ
		m_instancing.m_instanceNum, // 第⼆引数は要素数
		nullptr // 第三引数は初期値データ。初期値は指定しないので、今回はnullptr
	);

	// モデルの初期化データを設定する
	ModelInitData modelInitData;

	// tkmファイルのパスを指定
	modelInitData.m_tkmFilePath = "Assets/modelData/model/model.tkm";
	// 使⽤するシェーダーファイルのパスを指定
	modelInitData.m_fxFilePath = "Assets/shader/sample3DInstancing.fx";
	// 拡張SRVにストラクチャードバッファを渡す
	modelInitData.m_expandShaderResoruceView[0] = &m_instancing.m_worldMatrixSB;

	// スケルトンを指定する
	if (m_skeletonPointer) {	// スケルトンが初期化されていたら
		modelInitData.m_skeleton = m_skeletonPointer.get();
	}
	// モデルの上方向を指定
	modelInitData.m_modelUpAxis = modelUpAxis;

	// 設定したデータでモデルを初期化
	m_model.Init(modelInitData);
}

void ModelRender::InitOffScreenModel(
	const char* filePath,
	const int modelMaxNum,
	modelUpAxis::EnModelUpAxis modelUpAxis
)
{
	m_offScreen.m_instanceNum = modelMaxNum;

	// step-1 ⼈物のモデルを描画するレンダリングターゲットを初期化。
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_offScreen.m_drawModelRenderTarget.Create(
		512, // 横幅
		512, // 縦幅
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	// step-2 ⼈物のモデルを初期化。
	// モデルの初期化データを設定する。
	ModelInitData modelInitData;
	// tkmファイルのパスを指定。
	modelInitData.m_tkmFilePath = "Assets/modelData/model/model.tkm";
	// 通常の3Dモデル描画⽤のシェーダーを指定する。
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";

	// スケルトンを指定する
	if (m_skeletonPointer) {	// スケルトンが初期化されていたら
		modelInitData.m_skeleton = m_skeletonPointer.get();
	}

	// モデルの上方向を指定
	modelInitData.m_modelUpAxis = modelUpAxis;

	// 設定したデータでモデルを初期化。
	m_model.Init(modelInitData);

	// step-3 ⼈物のモデルを描画するためのカメラを作成。
	//m_offScreen.m_drawModelCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	m_offScreen.m_drawModelCamera.SetWidth(400.0f);
	m_offScreen.m_drawModelCamera.SetHeight(400.0f);
	//m_offScreen.m_drawModelCamera.SetNear(1.0f);
	//m_offScreen.m_drawModelCamera.SetFar(1000.0f);
	m_offScreen.m_drawModelCamera.SetPosition(0.0f, 300.0f, 0.0f);
	m_offScreen.m_drawModelCamera.SetTarget(0.0f, 0.0f, 0.0f);
	m_offScreen.m_drawModelCamera.SetUp({ 1.0f, 0.0f, 0.0f });
	m_offScreen.m_drawModelCamera.SetViewAngle(Math::DegToRad(1000.0f));
	m_offScreen.m_drawModelCamera.Update();

	//// step-3 ⼈物のモデルを描画するためのカメラを作成。
	//Camera drawHumanModelCamera;
	////drawHumanModelCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	//drawHumanModelCamera.SetWidth(400.0f);
	//drawHumanModelCamera.SetHeight(400.0f);
	//drawHumanModelCamera.SetNear(1.0f);
	//drawHumanModelCamera.SetFar(1000.0f);
	//drawHumanModelCamera.SetPosition(0.0f, 200.0f, 0.0f);
	//drawHumanModelCamera.SetTarget(0.0f, 0.0f, 0.0f);
	//drawHumanModelCamera.SetUp({ 0.0f, 1.0f, 0.0f });

	//m_offScreen.m_drawModelCamera.SetUp({ 1.0f, 0.0f, 0.0f });
	//m_offScreen.m_drawModelCamera.SetViewAngle(Math::DegToRad(1000.0f));

	//drawHumanModelCamera.Update();


	//// カメラの位置を設定
	//m_offScreen.m_drawModelCamera.SetPosition(0.0f, 600.0f, 0.0f);
	//// カメラの注視点を設定
	//m_offScreen.m_drawModelCamera.SetTarget(0.0f, 0.0f, 0.0f);
	//// カメラの上方向を設定
	//m_offScreen.m_drawModelCamera.SetUp({ 1.0f, 0.0f, 0.0f });
	//// カメラの画角を設定
	//m_offScreen.m_drawModelCamera.SetViewAngle(Math::DegToRad(1000.0f));
	//// ライトビュープロジェクション行列を計算
	//m_offScreen.m_drawModelCamera.Update();


	// モデルの座標を初期化
	m_offScreen.m_position = new Vector3[m_offScreen.m_instanceNum];
	m_offScreen.m_rotation = new Quaternion[m_offScreen.m_instanceNum];
	m_offScreen.m_scale = new Vector3[m_offScreen.m_instanceNum];
	// モデルの座標、回転、拡大率を初期化
	for (int instanceNum = 0; instanceNum < m_offScreen.m_instanceNum; instanceNum++) {
		// インスタンス全ての座標を原点、回転をデフォルト、拡大率を１倍に設定
		m_offScreen.m_position[instanceNum] = Vector3::Zero;
		m_offScreen.m_rotation[instanceNum] = Quaternion::Identity;
		m_offScreen.m_scale[instanceNum] = Vector3::One;
	}
	// インスタンシング描画のワールド⾏列関係の各種バッファを確保
	// まずは計算⽤のバッファをメインメモリ上に確保する
	m_offScreen.m_worldMatrixArray = new Matrix[m_offScreen.m_instanceNum];
	// 続いて、シェーダー側でワールド⾏列を使⽤するためのストラクチャードバッファをVRAM上に確保する
	m_offScreen.m_worldMatrixSB.Init(
		sizeof(Matrix), // 第⼀引数は１要素のサイズ
		m_offScreen.m_instanceNum, // 第⼆引数は要素数
		nullptr // 第三引数は初期値データ。初期値は指定しないので、今回はnullptr
	);

	// step-4 ⼈物のモデルのテクスチャを貼り付ける板ポリモデルを初期化。
	ModelInitData planeModelInitData;
	planeModelInitData.m_tkmFilePath = "Assets/modelData/plane/plane.tkm";
	// 【注⽬】インスタンシング描画⽤のシェーダーを指定する。
	planeModelInitData.m_fxFilePath = "Assets/shader/sample3DInstancing.fx";
	// 【注⽬】拡張SRVにストラクチャードバッファを渡す。
	planeModelInitData.m_expandShaderResoruceView[0] = &m_offScreen.m_worldMatrixSB;

	m_offScreen.m_boardPolyModel.Init(planeModelInitData);
	// 【注⽬】板ポリモデルのアルベドテクスチャを差し替える。
	m_offScreen.m_boardPolyModel.ChangeAlbedoMap("", m_offScreen.m_drawModelRenderTarget.GetRenderTargetTexture());
}

////////////////////////////////////////////////////////////
// 描画処理
////////////////////////////////////////////////////////////

void ModelRender::Render(RenderContext& renderContext)
{
	// 未初期化時
	if (m_finishInit == false) {
		return;
	}

	// インスタンシング描画
	if (true == CheckRenderType(EnRenderType::enInstancing)) {
		InstancingRender(renderContext);
		return;
	}

	// オフスクリーンレンダリング
	if (true == CheckRenderType(EnRenderType::enOffScreen)) {
		OffScreenRender(renderContext);
		return;
	}

	// モデルの描画
	m_model.Draw(renderContext);
}

void ModelRender::InstancingRender(RenderContext& renderContext)
{
	// インスタンシング描画
	m_model.DrawInstancing(renderContext, m_instancing.m_instanceNum);
}

void ModelRender::OffScreenRender(RenderContext& renderContext)
{
	// step-5 ⼈物のモデルを描画。
	// レンダリングターゲットとして利⽤できるまで待つ
	renderContext.WaitUntilToPossibleSetRenderTarget(m_offScreen.m_drawModelRenderTarget);
	// レンダリングターゲットを設定
	renderContext.SetRenderTargetAndViewport(m_offScreen.m_drawModelRenderTarget);
	// レンダリングターゲットをクリア
	renderContext.ClearRenderTargetView(m_offScreen.m_drawModelRenderTarget);
	// ⼈間モデルを描画。
	m_model.Draw(renderContext, m_offScreen.m_drawModelCamera);
	// レンダリングターゲットへの書き込み終了待ち
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_offScreen.m_drawModelRenderTarget);

	// step-6 レンダリングターゲットの設定をフレームバッファに戻す。
	// レンダリングターゲットをフレームバッファに戻す。
	renderContext.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	// ビューポートとシザリング矩形を指定する。
	renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	// step-7 板ポリモデルをインスタンシング描画。
	// 板ポリモデルをインスタンシング描画。
	m_offScreen.m_boardPolyModel.DrawInstancing(renderContext, m_offScreen.m_instanceNum);
}

void ModelRender::Update()
{
	// 未初期化時
	if (m_finishInit == false) {
		return;
	}

	// インスタンシング描画
	if (true == CheckRenderType(EnRenderType::enInstancing)) {
		InstancingUpdate();
		return;
	}

	// オフスクリーンレンダリング
	if (true == CheckRenderType(EnRenderType::enOffScreen)) {
		OffScreenUpdate();
		return;
	}

	// スウェー、しゃがみの処理
	SwayOrCrouching();

	// スケルトンを更新
	if (m_skeletonPointer) {	// スケルトンが初期化されていたら
		//SwayCharacter();
		m_skeletonPointer->Update(m_model.GetWorldMatrix());
	}
	// アニメーションを進める
	if (m_animationPointer) {	// アニメーションが初期化されていたら
		m_animationPointer->Progress(g_gameTime->GetFrameDeltaTime());
	}
	// モデルの座標更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

	// 他クラスの物の情報の更新
	UpdateOtherData();
}

void ModelRender::InstancingUpdate()
{
	// スケルトンを更新
	if (m_skeletonPointer) {	// スケルトンが初期化されていたら
		//SwayCharacter();
		m_skeletonPointer->Update(m_model.GetWorldMatrix());
	}
	// アニメーションを進める
	if (m_animationPointer) {	// アニメーションが初期化されていたら
		m_animationPointer->Progress(g_gameTime->GetFrameDeltaTime());
	}

	// ワールド⾏列を計算する
	for (int instanceNum = 0; instanceNum < m_instancing.m_instanceNum; instanceNum++) {
		m_instancing.m_worldMatrixArray[instanceNum] =
			m_model.CalcWorldMatrix(
				m_instancing.m_position[instanceNum],
				m_instancing.m_rotation[instanceNum],
				m_instancing.m_scale[instanceNum]
			);
	}

	// ワールド⾏列の内容をグラフィックメモリにコピー
	m_instancing.m_worldMatrixSB.Update(m_instancing.m_worldMatrixArray);
}

void ModelRender::OffScreenUpdate()
{
	// ワールド⾏列を計算する。
	for (int instanceNum = 0; instanceNum < m_offScreen.m_instanceNum; instanceNum++) {
		// ワールド⾏列を計算する。
		m_offScreen.m_worldMatrixArray[instanceNum] =
			m_model.CalcWorldMatrix(
				m_offScreen.m_position[instanceNum],
				m_offScreen.m_rotation[instanceNum],
				m_offScreen.m_scale[instanceNum]
			);
	}
	// ワールド⾏列の内容をグラフィックメモリに描画。
	m_offScreen.m_worldMatrixSB.Update(m_offScreen.m_worldMatrixArray);

	//Quaternion qRot;
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}

void ModelRender::UpdateOtherData()
{
	// 当たり判定の情報の更新
	if (m_skeletonPointer) {
		if (false == m_flagInitHitbox) {
			m_getHitbox->Create();
		}

		m_getHitbox->UpdateHitbox();
	}
}

void ModelRender::SwayOrCrouching()
{
	if (EnSwayController::enUp == m_swayController[EnXY::y]) {
		Crouching();
	}
	else {
		SwayCharacter();
	}
}

void ModelRender::SwayCharacter()
{
	// スケルトンがない場合
	if (!m_skeletonPointer) {
		return;
	}

	// 骨の名前でボーンIDを検索
	int boneId = m_skeletonPointer->FindBoneID(L"J_Bip_C_UpperChest");
	// 検索したボーンIDを使用して、ボーンを取得
	Bone* bone = m_skeletonPointer->GetBone(boneId);
	// ボーンのローカル行列を取得
	Matrix boneMatrix = bone->GetLocalMatrix();

	// ボーンの回転の軸になる前方向のベクトルを作成
	Vector3 vecFront = Vector3::Front;
	// ボーンの回転の軸になる右方向のベクトルを作成
	Vector3 vecRight = Vector3::Right;
	// ボーンの回転の軸に使用するベクトルをY軸に回すためのクォータニオンを作成
	Quaternion m_RotY;
	m_RotY.SetRotationY(0.6f); // 回転量を設定
	// ボーンの回転の軸になるベクトルをY軸で回転
	m_RotY.Apply(vecFront);
	m_RotY.Apply(vecRight);

	// スウェーの移動量を計算
	CheckSwayMove();

	// ボーンの回転情報を作成する行列を作成
	Matrix rotMatrixX, rotMatrixZ;
	// 回転情報を作成
	rotMatrixX.MakeRotationAxis(vecFront, -m_swayMove.x);
	rotMatrixZ.MakeRotationAxis(vecRight, m_swayMove.y);

	// 回転情報の行列を乗算し、ボーンを回転させる
	rotMatrixX *= rotMatrixZ;
	boneMatrix *= rotMatrixX;

	// 変更したボーン行列を設定
	bone->SetLocalMatrix(boneMatrix);
}

void ModelRender::CheckSwayMove()
{
	// 左右方向の移動量の計算
	// 入力: 左
	if (EnSwayController::enLeft == m_swayController[EnXY::x]) {
		m_swayMove.x -= SWAY_MOVE;

		// 移動量が上限を超えたか
		if (EnSwayController::enLeft >= m_swayMove.x) {
			m_swayMove.x = EnSwayController::enLeft;
		}
	}
	// 入力: 右
	else if (EnSwayController::enRight == m_swayController[EnXY::x]) {
		m_swayMove.x += SWAY_MOVE;

		// 移動量が上限を超えたか
		if (EnSwayController::enRight <= m_swayMove.x) {
			m_swayMove.x = EnSwayController::enRight;
		}
	}
	// 入力: なし
	else if (EnSwayController::enNotMove == m_swayController[EnXY::x]) {
		// 右方向に移動している
		if (EnSwayController::enNotMove < m_swayMove.x) {
			m_swayMove.x -= SWAY_MOVE;

			// 逆側に移動したか
			if (EnSwayController::enNotMove > m_swayMove.x) {
				m_swayMove.x = EnSwayController::enNotMove;
			}
		}
		// 左方向に移動している
		else if (EnSwayController::enNotMove > m_swayMove.x) {
			m_swayMove.x += SWAY_MOVE;

			// 逆側に移動したか
			if (EnSwayController::enNotMove < m_swayMove.x) {
				m_swayMove.x = EnSwayController::enNotMove;
			}
		}
	}

	// 上下方向の移動量の計算
	// 入力: 上
	if (EnSwayController::enUp == m_swayController[EnXY::y]) {
		m_swayMove.y += SWAY_MOVE;

		// 移動量が上限を超えたか
		if (EnSwayController::enUp <= m_swayMove.y) {
			m_swayMove.y = EnSwayController::enUp;
		}
	}
	// 入力: 下
	else if (EnSwayController::enDown == m_swayController[EnXY::y]) {
		m_swayMove.y -= SWAY_MOVE;

		// 移動量が上限を超えたか
		if (EnSwayController::enDown >= m_swayMove.y) {
			m_swayMove.y = EnSwayController::enDown;
		}
	}
	// 入力: なし
	else if (EnSwayController::enNotMove == m_swayController[EnXY::y]) {
		// 上方向に移動している
		if (EnSwayController::enNotMove < m_swayMove.y) {
			m_swayMove.y -= SWAY_MOVE;

			// 逆側に移動したか
			if (EnSwayController::enNotMove > m_swayMove.y) {
				m_swayMove.y = EnSwayController::enNotMove;
			}
		}
		// 下方向に移動している
		else if (EnSwayController::enNotMove > m_swayMove.y) {
			m_swayMove.y += SWAY_MOVE;

			// 逆側に移動したか
			if (EnSwayController::enNotMove < m_swayMove.y) {
				m_swayMove.y = EnSwayController::enNotMove;
			}
		}
	}
}


void ModelRender::Crouching()
{

}