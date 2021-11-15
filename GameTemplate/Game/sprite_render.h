#pragma once



namespace SRns //SpriteRenderクラス関連のnamespace constant
{
	////////////////////////////////////////////////////////////
	// 画像の色
	////////////////////////////////////////////////////////////

	const Vector4 COLOR_NORMAL = { 1.0f,1.0f,1.0f,1.0f };	//普通
	const Vector4 COLOR_GRAY = { 0.5f,0.5f,0.5f,1.0f };		//灰色

	////////////////////////////////////////////////////////////
	// 透明度
	////////////////////////////////////////////////////////////

	const float TRANSPARENCY_0 = 0.0f;		//0%
	const float TRANSPARENCY_100 = 1.0f;	//100%
}



class SpriteRender : public IGameObject
{
public:
    SpriteRender();
    ~SpriteRender();
    bool Start() override final;
    void AlwaysUpdate() override final;
    void Render(RenderContext& renderContext) override final;


public:
	/**
	 * @brief 初期化
	 * @param texFilePath ddsファイルパス
	 * @param w 横幅
	 * @param h 縦幅
	 * 縦幅と横幅は、すべて同じサイズの画像で作っているため、デフォルト引数にしている。
	 * @param alphaBlendMode アルファブレンディングモード
	 * 基本的に透過画像を使用するのでAlphaBlendMode_Transをデフォルト引数にしている。
	 * @param pivot ピボット（基点）
	*/
	void Init(const char* texFilePath, float w = 1280.0f, float h = 720.0f,
		AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans, const Vector2& pivot = { 0.5f,0.5f });

	/**
	 * @brief 場所を設定（{0.0f,0.0f}が中心）
	 * @param pos 場所
	*/
	void SetPosition(const Vector2& pos)
	{
		m_position.x = pos.x;
		m_position.y = pos.y;
	}

	/**
	 * @brief X座標の場所を設定
	 * @param pos X座標の場所
	*/
	void SetPositionX(const float pos)
	{
		m_position.x = pos;
	}

	/**
	 * @brief Y座標の場所を設定
	 * @param pos Y座標の場所
	*/
	void SetPositionY(const float pos)
	{
		m_position.y = pos;
	}

	/**
	 * @brief 回転量を設定
	 * @param rot 回転量
	*/
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	
	/**
	 * @brief 拡大率を設定
	 * @param scale 拡大率
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/**
	 * @brief ピボット（基点）を設定
	 * {0.0f,0.0f}が左下
	 * {0.5f,0.5f}が中央
	 * {1.0f,1.0f}が右上
	 * @param pivot ピボット（基点）
	*/
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
	}

	/**
	 * @brief スプライトに色を乗算する関数
	 * アルファ値を掛けて透過するときは、Initするときに
	 * アルファブレンディングモードを透過合成モードにしておくこと
	 * @param mulColor 乗算する色
	 * mulColor.x = Rカラー
	 * mulColor.y = Gカラー
	 * mulColor.z = Bカラー
	 * mulColor.w = 透過
	*/
	void SetMulColor(const Vector4& mulColor)
	{
		m_sprite.SetMulColor(mulColor);
	}

	/**
	 * @brief スプライトのmulColor.wを変更する関数
	 * @param mulColorW mulColor.wに乗算する色
	*/
	void SetMulColorW(const float mulColorW)
	{
		m_sprite.SetMulColorW(mulColorW);
	}

	/**
	 * @brief 場所を取得
	 * @return 場所
	*/
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/**
	 * @brief スプライトのmulColor.wを取得
	 * @return スプライトのmulColor.w
	*/
	const float GetMulColorW()
	{
		return m_sprite.GetMulColorW();
	}


private: //data menber
	Sprite m_sprite;							//スプライトクラス
	SpriteInitData m_initData;					//スプライトの初期化データ。構造体。

	Vector3 m_position = { 0.0f,0.0f,0.0f };	//場所
	Quaternion m_rotation = g_quatIdentity;		//回転
	//座標系の関係で左右反転するから
	//xの拡大を反転させる
	Vector3 m_scale = { 1.0f,1.0f,1.0f };		//拡大
	Vector2 m_pivot = { 0.5f,0.5f };			//ピボット（基点）
};