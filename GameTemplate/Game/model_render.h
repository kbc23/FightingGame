#pragma once
#include <Model.h>

#include "shadow.h"

//#include "constant.h"

////////////////////////////////////////////////////////////
// メモ
// トゥーンレンダリングに使用する画像などのパスを読み込んでいるところ
// NullTextureMaps.cpp
////////////////////////////////////////////////////////////

//このmodel_render.hとmodel_render.cppに存在するマジックナンバーを定数などに変更しておくこと。



//ライト用の構造体を定義する
struct Light {
	//ディレクションライト
	Vector3 dirDirection; //ライトの方向
	//HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、
	//C++側にはパディングを埋めておく。
	float pad;
	Vector3 dirColor;
	float pad1;

	//ポイントライト
	Vector3 ptPosition; //位置
	float pad2;			//パディング
	Vector3 ptColor;	//カラー
	float ptRange;		//影響範囲

	//スポットライト
	Vector3 spPosition;		//位置
	float pad3;				//パディング
	Vector3 spColor;		//カラー
	float spRange;			//影響範囲
	Vector3 spDirection;	//射出方向
	float spAngle;			//射出角度

	//構造体に視点の位置を追加する
	Vector3 eyePos; //視点の位置
	float pad4;

	//環境光
	Vector3 ambientLight;   // アンビエントライト
	float pad5;

	//半球ライト
	Vector3 groundColor;	//地面色
	float pad6;
	Vector3 skyColor;		//天球色
	float pad7;
	Vector3 groundNormal;	//地面の法線

	float pad8;
	Matrix shadowCamera;
};



class ModelRender : public IGameObject
{
public:
    ModelRender();
    ~ModelRender();
	bool Start() override final;
    void Update() override final;
	void Render(RenderContext& renderContext) override final;



public:
	/**
	 * @brief 初期化
	 * @param filePath tkmファイルのファイルパス
	 * @param modelUpAxis モデルの上方向
	 * @param animationClip アニメーションクリップ
	 * @param maxAnimationClipNum アニメーションクリップの最大数
	*/
	void Init(const char* filePath, bool flagShadowReceiver = false, bool flagShadow = false,
		modelUpAxis::EnModelUpAxis modelUpAxis = modelUpAxis::enModelUpAxisZ,
		AnimationClip* animationClip = nullptr,
		int maxAnimationClipNum = 0
	);



private:
	/**
	 * @brief モデルの初期化
	 * @param filePath tkmファイルのファイルパス
	 * @param  モデルの上方向
	*/
	void InitModel(const char* filePath, bool flagShadowReceiver, bool flagShadow,
		modelUpAxis::EnModelUpAxis
	);

	/**
	 * @brief ディレクションライトの初期化
	*/
	void InitDirectionLight();

	/**
	 * @brief ポイントライトの初期化
	*/
	void InitPointLight();

	/**
	 * @brief スポットライトの初期化
	*/
	void InitSpotLight();

	/**
	 * @brief 環境光の初期化
	*/
	void InitAmbientLight();

	/**
	 * @brief 半球ライトの初期化
	*/
	void InitHemiLight();

	/**
	 * @brief スケルトンの初期化
	 * @param filePath ファイルパス
	 * @return 
	*/
	bool InitSkeleton(const char* filePath);

	/**
	 * @brief アニメーションの初期化
	 * @param animationClip アニメーションクリップ
	 * @param maxAnimationClipNum アニメーションクリップの総数
	*/
	void InitAnimation(AnimationClip* animationClip, int maxAnimationClipNum);


public:
	/**
	 * @brief アニメーションを再生する
	 * @param animationNumber 登録しているアニメーションクリップの番号
	 * @param interpolateTime 補完時間（単位：秒）
	*/
	void PlayAnimation(int animationNumber, float interpolateTime = 0.2f)
	{
		m_animationPointer->Play(animationNumber, interpolateTime);
	}

	/**
	 * @brief アニメーションを再生中か
	 * @return 再生中か
	*/
	const bool IsPlayingAnimation() const
	{
		return m_animationPointer->IsPlaying();
	}

	/**
	 * @brief アニメーションの初期化が完了しているか
	 * @return 完了しているか
	*/
	const bool IsInited() const
	{
		return m_animationPointer->IsInited();
	}



public: //Get関数
	/**
	 * @brief 位置を取得
	 * @return 位置
	*/
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/**
	 * @brief 回転量を取得
	 * @return 回転量
	*/
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/**
	 * @brief 拡大率を取得
	 * @return 拡大率
	*/
	const Vector3& GetScale() const
	{
		return m_scale;
	}



public: //Set関数
	/**
	 * @brief 位置を設定
	 * @param vec 位置
	*/
	void SetPosition(const Vector3& position)
	{
		m_position = position;

		if (true == m_flagShadow) {
			m_shadowModel->SetPosition(m_position);
		}
	}

	/**
	 * @brief Y座標を指定した量だけ上昇
	 * @param position 上昇する量
	*/
	void UpPositionY(const float position)
	{
		m_position.y += position;
	}

	/**
	 * @brief Y座標を指定した量だけ下降
	 * @param position 下降する量
	*/
	void DownPositionY(const float position)
	{
		m_position.y -= position;
	}

	/**
	 * @brief Y座標を0.0fに設定する
	*/
	void ResetPositionY()
	{
		m_position.y = 0.0f;
	}

	/**
	 * @brief 回転量を設定
	 * @param rotation 回転量
	*/
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;

		if (true == m_flagShadow) {
			m_shadowModel->SetRotation(m_rotation);
		}
	}

	/**
	 * @brief X軸の回転量を設定
	 * @param rotation X軸の回転量
	*/
	void SetRotationX(const float rotation) {
		m_rotation.SetRotationX(rotation);
	}

	/**
	 * @brief 拡大率を設定
	 * @param scale 拡大率
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;

		if (true == m_flagShadow) {
			m_shadowModel->SetScale(m_scale);
		}
	}





private: //data menber
    Model m_model;
	Shadow* m_shadowModel = nullptr; //影の描画処理
	std::unique_ptr<Skeleton> m_skeletonPointer; //スケルトンのポインター
	std::unique_ptr<Animation> m_animationPointer; //アニメーションのポインター


    const char* m_tkmFilePath = nullptr; //tkmファイルのファイルパス
	Light m_light;

	Vector3 m_position = g_vec3Zero;			//位置
	Quaternion m_rotation = g_quatIdentity;		//回転
	Vector3 m_scale = g_vec3One;				//拡大

	bool m_finishInit = false; //初期化が終わったか
	bool m_flagShadow = false; //影が発生するか
};