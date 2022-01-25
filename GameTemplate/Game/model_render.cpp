#include "stdafx.h"
#include "model_render.h"

#include "shadow_map.h"
#include "shadow_light_camera.h"



namespace // constant
{
	const float SWAY_MOVE = 0.2f;
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
	InitInstancingModel(filePath, modelUpAxis);
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


	// 通常描画
	if (EnRenderType::enNormal == m_flagSetRenderType) {
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
	// インスタンス描画
	else if (EnRenderType::enInstancing == m_flagSetRenderType) {
		// スケルトンのリソースを確保
		m_instancing.m_skeletonPointer.resize(500);

		for (int i = 0; i < 500; i++) {
			m_instancing.m_skeletonPointer[i].reset(new Skeleton);

			// スケルトンのデータの読み込み
			bool isInited = m_instancing.m_skeletonPointer[i]->Init(skeletonFilePath.c_str());

			// 初期化に成功したか
			// 成功
			if (isInited) {
				return true;
			}
			// 失敗
			else {
				// スケルトンのリソースを解放
				m_instancing.m_skeletonPointer[i].reset();
				return false;
			}
		}
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

	return;


	// 通常描画
	if (EnRenderType::enNormal == m_flagSetRenderType) {
		// アニメーションのリソースを確保
		m_animationPointer.reset(new Animation);
		// アニメーションを初期化
		m_animationPointer->Init(
			*m_skeletonPointer,
			animationClip,
			maxAnimationClipNum
		);
	}
	// インスタンス描画
	else if (EnRenderType::enInstancing == m_flagSetRenderType) {
		for (int i = 0; i < 500; i++) {
			// アニメーションのリソースを確保
			m_instancing.m_animationPointer[i].reset(new Animation);
			// アニメーションを初期化
			m_instancing.m_animationPointer[i]->Init(
				*m_instancing.m_skeletonPointer[i],
				animationClip,
				maxAnimationClipNum
			);
		}
	}
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

void ModelRender::InitInstancingModel(const char* filePath, modelUpAxis::EnModelUpAxis modelUpAxis)
{
	// step-1 500体分のモデルの座標を計算する。
	m_instancing.m_position = new Vector3[500];
	m_instancing.m_rotation = new Quaternion[500];
	m_instancing.m_scale = new Vector3[500];
	int humanNo = 0;
	for (int x = 0; x < 50; x++) {
		for (int y = 0; y < 10; y++) {
			// 1000体のモデルが綺麗に並ぶように、座標を計算する。
			m_instancing.m_position[humanNo].x = -2400.0f + 100.0f * x;
			m_instancing.m_position[humanNo].y = -1250.0f + 250.0f * y;
			m_instancing.m_position[humanNo].z = 0.0f;

			//m_instancing.m_rotation[humanNo].SetRotationDegX(90.0f);
			humanNo++;
		}
	}

	// step-2 500体分のワールド⾏列関係の各種バッファを確保。
	// まずは計算⽤のバッファをメインメモリ上に確保する。
	m_instancing.m_worldMatrixArray = new Matrix[500];
	// 続いて、シェーダー側でワールド⾏列を使⽤するためのストラクチャードバッファをVRAM上に確保する。
	//StructuredBuffer worldMatrixSB;
	m_instancing.m_worldMatrixSB.Init(
		sizeof(Matrix), // 第⼀引数は１要素のサイズ。
		500, // 第⼆引数は要素数。
		nullptr // 第三引数は初期値データ。初期値は指定しないので、今回はnullptr。
	);

	// step-3 ⼈物のモデルを初期化。
	// モデルの初期化データを設定する。
	ModelInitData modelInitData;

	//modelInitData.m_vsSkinEntryPointFunc = "VSMain";

	// tkmファイルのパスを指定。
	modelInitData.m_tkmFilePath = "Assets/modelData/model/model.tkm";
	// 使⽤するシェーダーファイルのパスを指定。
	modelInitData.m_fxFilePath = "Assets/shader/sample3DInstancing.fx";
	// 【注⽬】拡張SRVにストラクチャードバッファを渡す。
	modelInitData.m_expandShaderResoruceView[0] = &m_instancing.m_worldMatrixSB;

	// スケルトンを指定する。
	if (m_skeletonPointer) {	// スケルトンが初期化されていたら
		modelInitData.m_skeleton = m_skeletonPointer.get();
	}
	// モデルの上方向を指定
	modelInitData.m_modelUpAxis = modelUpAxis;

	// 設定したデータでモデルを初期化。
	m_model.Init(modelInitData);

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
	if (EnRenderType::enInstancing == m_renderType) {
		InstancingRender(renderContext);
		return;
	}

	// モデルの描画
	m_model.Draw(renderContext);
}

void ModelRender::InstancingRender(RenderContext& renderContext)
{
	// step-6 ⼈物のモデルをインスタンシング描画。
	m_model.DrawInstancing(renderContext, 500);
}

void ModelRender::Update()
{
	// 未初期化時
	if (m_finishInit == false) {
		return;
	}

	// インスタンシング描画
	if (EnRenderType::enInstancing == m_renderType) {
		InstancingUpdate();
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


	// step-4 ワールド⾏列を計算する。
	for (int i = 0; i < 500; i++) {
		// ワールド⾏列を計算する。
		m_instancing.m_worldMatrixArray[i] =
			m_model.CalcWorldMatrix(
				m_instancing.m_position[i],
				m_instancing.m_rotation[i],
				m_instancing.m_scale[i]
			);
	}

	// step-5 ワールド⾏列の内容をグラフィックメモリにコピー。
	m_instancing.m_worldMatrixSB.Update(m_instancing.m_worldMatrixArray);
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