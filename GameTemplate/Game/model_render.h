#pragma once
#include <Model.h>

#include "shadow.h"
#include "hitbox.h"

////////////////////////////////////////////////////////////
// メモ
// トゥーンレンダリングに使用する画像などのパスを読み込んでいるところ
// NullTextureMaps.cpp
////////////////////////////////////////////////////////////



// ライト用の構造体を定義する
struct Light {
	// ディレクションライト
	Vector3 dirDirection; // ライトの方向
	// HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、
	// C++側にはパディングを埋めておく。
	float pad;
	Vector3 dirColor;
	float pad1;

	// ポイントライト
	Vector3 ptPosition; // 位置
	float pad2;			// パディング
	Vector3 ptColor;	// カラー
	float ptRange;		// 影響範囲

	// スポットライト
	Vector3 spPosition;		// 位置
	float pad3;				// パディング
	Vector3 spColor;		// カラー
	float spRange;			// 影響範囲
	Vector3 spDirection;	// 射出方向
	float spAngle;			// 射出角度

	// 構造体に視点の位置を追加する
	Vector3 eyePos; // 視点の位置
	float pad4;

	// 環境光
	Vector3 ambientLight;   // アンビエントライト
	float pad5;

	// 半球ライト
	Vector3 groundColor;	// 地面色
	float pad6;
	Vector3 skyColor;		// 天球色
	float pad7;
	Vector3 groundNormal;	// 地面の法線

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
	 * @param flagShadowReceiver 影の影響を受けるか
	 * @param flagShadow 影を描画するか
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
	 * @param flagShadowReceiver 影の影響を受けるか
	 * @param flagShadow 影を描画するか
	 * @param modelUpAxis モデルの上方向
	*/
	void InitModel(const char* filePath, bool flagShadowReceiver, bool flagShadow,
		modelUpAxis::EnModelUpAxis modelUpAxis
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

	/**
	 * @brief 他のクラスの物だが、モデルと位置がずれるのを防ぐためにここで処理をおこなう
	*/
	void UpdateOtherData();


private:
	/**
	 * @brief スウェーの処理
	*/
	void SwayCharacter();


	void CheckSwayMove();

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


public: // Get関数
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

	/**
	 * @brief 指定したボーンを取得
	 * @param boneName ボーンの名前
	 * @return ボーン
	*/
	Bone& GetBone(const wchar_t boneName) const
	{
		// ボーンの名前を指定してボーンIDを検索
		int boneId = m_skeletonPointer->FindBoneID(&boneName);
		// 検索したボーンIDを使用して、ボーンを取得
		return *m_skeletonPointer->GetBone(boneId);
	}

	/**
	 * @brief スケルトンを取得
	 * @return スケルトン
	*/
	Skeleton& GetSkeleton() const
	{
		return *m_skeletonPointer;
	}

	Matrix& GetModelWorldMatrix() const
	{
		m_model.GetWorldMatrix();
	}


public: // Set関数
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

	/**
	 * @brief Hitboxクラスの位置を更新する際、このクラスでしたいので持ってくる
	 * （関数名変えること）
	 * @param hitbox Hitboxクラスのインスタンス
	*/
	void SetTest(Hitbox& hitbox)
	{
		m_getHitbox = &hitbox;
	}

	/**
	 * @brief スウェーの移動方向
	 * @param swayMove 入力情報
	*/
	void SetSwayMove(const Vector2& swayMoveController)
	{
		// 左右方向の確認
		// 入力: 右
		if (EnSwayController::enNotMove < swayMoveController.x) {
			m_swayController[EnXY::x] = EnSwayController::enRight;
		}
		// 入力: 左
		else if (EnSwayController::enNotMove > swayMoveController.x) {
			m_swayController[EnXY::x] = EnSwayController::enLeft;
		}
		// 入力: なし
		else {
			m_swayController[EnXY::x] = EnSwayController::enNotMove;
		}
		// 上下方向の確認
		// 入力: 上
		if (EnSwayController::enNotMove < swayMoveController.y) {
			m_swayController[EnXY::y] = EnSwayController::enUp;
		}
		// 入力: 下
		else if (EnSwayController::enNotMove > swayMoveController.y) {
			m_swayController[EnXY::y] = EnSwayController::enDown;
		}
		// 入力: なし
		else {
			m_swayController[EnXY::y] = EnSwayController::enNotMove;
		}
	}


private: // enum
	/**
	 * @brief スウェーの移動方向
	*/
	enum EnSwayController
	{
		enLeft = -1,
		enRight = 1,
		enUp = 1,
		enDown = -1,
		enNotMove = 0
	};

	/**
	 * @brief 仮設置
	*/
	enum EnXY
	{
		x,
		y,
		MaxXY
	};

	int m_swayController[EnXY::MaxXY] = { EnSwayController::enNotMove }; // スウェーの移動方向


private: // data member
    Model m_model;
	Shadow* m_shadowModel = nullptr; // 影の描画処理
	std::unique_ptr<Skeleton> m_skeletonPointer; // スケルトンのポインター
	std::unique_ptr<Animation> m_animationPointer; // アニメーションのポインター

    const char* m_tkmFilePath = nullptr; // tkmファイルのファイルパス
	Light m_light;

	Vector3 m_position = g_vec3Zero;			// 位置
	Quaternion m_rotation = g_quatIdentity;		// 回転
	Vector3 m_scale = g_vec3One;				// 拡大

	bool m_finishInit = false; // 初期化が終わったか
	bool m_flagShadow = false; // 影が発生するか

	Hitbox* m_getHitbox = nullptr; // 取得したHitboxクラスのインスタンス

	bool m_flagInitHitbox = false; // 当たり判定を初期化したか

	Vector2 m_swayMove = g_vec2Zero; // スウェーの移動量
};