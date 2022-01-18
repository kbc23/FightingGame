#include "stdafx.h"
#include "model_render.h"

#include "shadow_map.h"
#include "shadow_light_camera.h"



namespace // constant
{
	const float SWAY_MOVE = 0.2f;
}



ModelRender::ModelRender()
{

}

ModelRender::~ModelRender()
{
	DeleteGO(m_shadowModel);
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

void ModelRender::AudienceInit(const char* filePath, bool flagShadowReceiver, bool flagShadow,
	modelUpAxis::EnModelUpAxis modelUpAxis,
	AnimationClip* animationClip,
	int maxAnimationClipNum
)
{
	// tkmファイルのファイルパスを保持
	m_tkmFilePath = filePath;
	// スケルトンのデータの読み込み
	InitSkeleton(filePath);
	// モデルの初期化
	InitAudienceModel(filePath, flagShadowReceiver, flagShadow, modelUpAxis);
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
		modelInitData.m_expandConstantBuffer = &m_light;
		modelInitData.m_expandConstantBufferSize = sizeof(m_light);
	}
	else {
		modelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";
		// シャドウマップを拡張SRVに設定する。
		modelInitData.m_expandShaderResoruceView = 
			&ShadowMap::GetInstance()->GetShadowMap().GetRenderTargetTexture();

		// ライトビュープロジェクション行列を拡張定数バッファに設定する。
		modelInitData.m_expandConstantBuffer = &m_light;
		modelInitData.m_expandConstantBufferSize = sizeof(m_light);
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

void ModelRender::InitAudienceModel(const char* filePath, bool flagShadowReceiver, bool flagShadow,
	modelUpAxis::EnModelUpAxis modelUpAxis
)
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

	m_light.shadowCamera = ShadowLightCamera::GetInstance()->GetShadowLightCamera().GetViewProjectionMatrix();



	m_worldMatrixArray = new Matrix[100];

	m_worldMatrixSB.Init(
		sizeof(Matrix), // 第⼀引数は１要素のサイズ。
		100, // 第⼆引数は要素数。
		nullptr // 第三引数は初期値データ。初期値は指定しないので、今回はnullptr。
	);



	// 3Dモデルをロードするための情報を設定する
	// モデルの初期化するための情報を設定
	ModelInitData modelInitData;
	// tkmファイルのファイルパスを設定
	modelInitData.m_tkmFilePath = filePath;

	// 使⽤するシェーダーファイルのパスを指定。
	modelInitData.m_fxFilePath = "Assets/shader/sample3DInstancing.fx";
	// 【注⽬】拡張SRVにストラクチャードバッファを渡す。
	modelInitData.m_expandShaderResoruceView[0] = m_worldMatrixSB;


	//// 使用するシェーダーファイルパスを設定

	//// 影を受けるか
	//if (false == flagShadowReceiver) {
	//	modelInitData.m_fxFilePath = "Assets/shader/model.fx";

	//	// ライトの情報を定数バッファとしてディスクリプタヒープに
	//	// 登録するためにモデルの初期化情報として渡す。
	//	modelInitData.m_expandConstantBuffer = &m_light;
	//	modelInitData.m_expandConstantBufferSize = sizeof(m_light);
	//}
	//else {
	//	modelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";
	//	// シャドウマップを拡張SRVに設定する。
	//	modelInitData.m_expandShaderResoruceView =
	//		&ShadowMap::GetInstance()->GetShadowMap().GetRenderTargetTexture();

	//	// ライトビュープロジェクション行列を拡張定数バッファに設定する。
	//	modelInitData.m_expandConstantBuffer = &m_light;
	//	modelInitData.m_expandConstantBufferSize = sizeof(m_light);
	//}
	//// 影を生成するか
	//if (true == flagShadow) {
	//	m_shadowModel = NewGO<Shadow>(0);

	//	m_shadowModel->Init(filePath, *m_skeletonPointer);

	//	m_flagShadow = true;
	//}

	// スケルトンを指定する。
	if (m_skeletonPointer) {	// スケルトンが初期化されていたら
		modelInitData.m_skeleton = m_skeletonPointer.get();
	}
	// モデルの上方向を指定
	modelInitData.m_modelUpAxis = modelUpAxis;

	// 初期化情報を使ってモデル表示処理を初期化する
	m_model.Init(modelInitData);
}

//////////////////////////////
// ライトごとの初期化
//////////////////////////////

void ModelRender::InitDirectionLight()
{
	// ライトは斜め上から当たっている。
	m_light.dirDirection.x = 1.0f;
	m_light.dirDirection.y = -1.0f;
	m_light.dirDirection.z = -1.0f;
	// 正規化する。
	m_light.dirDirection.Normalize();
	// ライトのカラーの設定（ライトの強さ）
	m_light.dirColor.x = 1.5f;
	m_light.dirColor.y = 1.5f;
	m_light.dirColor.z = 1.5f;

	// 視点の位置を設定
	m_light.eyePos = g_camera3D->GetPosition();
}


void ModelRender::InitPointLight()
{
	// ポイントライトの初期座標を設定する
	m_light.ptPosition.x = 0.0f;
	m_light.ptPosition.y = 50.0f;
	m_light.ptPosition.z = 50.0f;

	// ポイントライトの初期カラーを設定する
	m_light.ptColor.x = 15.0f;
	m_light.ptColor.y = 0.0f;
	m_light.ptColor.z = 240.0f;

	// ポイントライトの影響範囲を設定する
	m_light.ptRange = 100.0f;
}

void ModelRender::InitSpotLight()
{
	// 初期座標
	m_light.spPosition.x = 0.0f;
	m_light.spPosition.y = 50.0f;
	m_light.spPosition.z = -150.0f;

	// ライトのカラー
	m_light.spColor.x = 200.0f;
	m_light.spColor.y = 10.0f;
	m_light.spColor.z = 10.0f;
	// 初期方向は斜め下にする。
	m_light.spDirection.x = 1.0f;
	m_light.spDirection.y = -1.0f;
	m_light.spDirection.z = 1.0f;
	// 正規化
	m_light.spDirection.Normalize();
	// 射出範囲は300
	m_light.spRange = 300.0f;
	// 射出角度は25度
	m_light.spAngle = Math::DegToRad(25.0f);
}

void ModelRender::InitAmbientLight()
{
	// 環境光
	m_light.ambientLight.x = 0.05f;
	m_light.ambientLight.y = 0.05f;
	m_light.ambientLight.z = 0.05f;
}

void ModelRender::InitHemiLight()
{
	// 地面色、天球色、地面の法線のデータを設定する
	m_light.groundColor.x = 0.7f;
	m_light.groundColor.y = 0.5f;
	m_light.groundColor.z = 0.3f;

	// 天球色を設定
	m_light.skyColor.x = 0.15f;
	m_light.skyColor.y = 0.7f;
	m_light.skyColor.z = 0.95f;

	// 地面の法線を設定
	m_light.groundNormal.x = 0.0f;
	m_light.groundNormal.y = 1.0f;
	m_light.groundNormal.z = 0.0f;
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

	// モデルの描画
	m_model.Draw(renderContext);
}

void ModelRender::Update()
{
	// 未初期化時
	if (m_finishInit == false) {
		return;
	}

	if (true == m_flagAudience) {
		UpdateOtherData();
		return;
	}

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

void ModelRender::AudienceUpdate()
{
	// step-4 ワールド⾏列を計算する。
	for (int i = 0; i < 100; i++) {
		// ワールド⾏列を計算する。
		m_worldMatrixArray[i] = m_model.CalcWorldMatrix(m_audiencePos[i], m_rotation,
			g_vec3One);
	}

	// step-5 ワールド⾏列の内容をグラフィックメモリにコピー。
	m_worldMatrixSB.Update(m_worldMatrixArray);

	// step-6 ⼈物のモデルをインスタンシグ描画。
	//m_model.DrawInstancing(renderContext, numHumanModel);





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

}

void ModelRender::UpdateOtherData()
{
	if (true == m_flagAudience) {
		return;
	}

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