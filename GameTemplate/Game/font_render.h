#pragma once

class FontRender : public IGameObject
{
public:
    FontRender();
    ~FontRender();
    bool Start() override final;
    void Update() override final;
    void Render(RenderContext& renderContext) override final;


public:
    /**
     * @brief 初期化関数
     * @param text 表示するテキスト
     * @param position 座標。{ 0.0f,0.0f }が中心
     * @param color カラー
     * @param rotation 回転
     * @param scale 拡大
     * @param pivot ピボット（基点）
    */
    void Init(
        const wchar_t* text,
        const Vector2 position = { 0.0f,0.0f },
        const float scale = 1.0f,
        const Vector4 color = { 0.0f,0.0f,0.0f,1.0f },
        const float rotation = 0.0f,
        const Vector2 pivot = { 0.5f,0.5f }
    );


public: //Set関数
    /**
     * @brief 表示するテキストを設定
     * @param text テキスト
    */
    void SetText(const wchar_t* text)
    {
        swprintf_s(m_text, text);
    }

    /**
     * overload
     * @brief 表示するテキストを設定（int型）
     * @param num テキスト（int型）
    */
    void SetText(const int num)
    {
        swprintf_s(m_text, L"%d", num);
    }

    /**
     * @brief 表示するテキストの位置を設定
     * @param vec 表示する位置
    */
    void SetPosition(const Vector2& vec)
    {
        m_position = vec;
    }

    /**
     * @brief スケールを設定
     * @param scale スケール
    */
    void SetScale(const float scale)
    {
        m_scale = scale;
    }

    /**
     * @brief 表示するテキストのピボットを設定
     * @param vec ピボット
    */
    void SetPivot(const Vector2& vec)
    {
        m_pivot = vec;
    }

    /**
     * @brief X座標の位置を設定
     * @param position X座標の位置
    */
    void SetPositionX(const float position)
    {
        m_position.x = position;
    }

    /**
     * @brief X座標の位置を取得
     * @return X座標の位置
    */
    const float& GetPositionX() const
    {
        return m_position.x;
    }


private: //constant
    static const int m_MAX_TEXT_SIZE = 256;

private: // data member
    Font m_font;
    wchar_t m_text[m_MAX_TEXT_SIZE] = L""; //表示するテキスト
    Vector2 m_position = { 0.0f,0.0f }; //表示する場所
    Vector4 m_color = { 0.0f,0.0f,0.0f,1.0f }; //文字のカラー（黒色）
    float m_rotation = 0.0f; //回転
    float m_scale = 1.0f; //拡大率
    Vector2 m_pivot = { 0.5f,0.5f }; //ピボット（基点）
};