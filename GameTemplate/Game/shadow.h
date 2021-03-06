#pragma once

class Shadow : public IGameObject
{
public:
    Shadow();
    ~Shadow();
    bool Start() override final;
    void Update() override final;
    void ShadowRender(RenderContext& renderContext) override final;

    void Init(const char* filePath, Skeleton& skeleton);
    


public: //Get関数
    /**
     * @brief 位置を設定
     * @param vec 位置
    */
    void SetPosition(const Vector3& position)
    {
        m_position = position;
    }

    /**
     * @brief 回転量を設定
     * @param rotation 回転量
    */
    void SetRotation(const Quaternion& rotation)
    {
        m_rotation = rotation;
    }

    /**
     * @brief 拡大率を設定
     * @param scale 拡大率
    */
    void SetScale(const Vector3& scale)
    {
        m_scale = scale;
    }


private: // data member
    Model m_shadowModel;

    bool m_finishInit = false; // 初期化が終わったか

    Vector3 m_position = g_vec3Zero;			// 位置
    Quaternion m_rotation = g_quatIdentity;		// 回転
    Vector3 m_scale = g_vec3One;				// 拡大
};