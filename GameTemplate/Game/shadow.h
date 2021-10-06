#pragma once

class Shadow : public IGameObject
{
public:
    Shadow();
    ~Shadow();
    bool Start() override final;
    void Update() override final;
    void ShadowRender(RenderContext& renderContext) override final;

    void Init(const char* filePath);
    


public: //GetŠÖ”
    /**
     * @brief ˆÊ’u‚ğİ’è
     * @param vec ˆÊ’u
    */
    void SetPosition(const Vector3& position)
    {
        m_position = position;
    }

    /**
     * @brief ‰ñ“]—Ê‚ğİ’è
     * @param rotation ‰ñ“]—Ê
    */
    void SetRotation(const Quaternion& rotation)
    {
        m_rotation = rotation;
    }

    /**
     * @brief Šg‘å—¦‚ğİ’è
     * @param scale Šg‘å—¦
    */
    void SetScale(const Vector3& scale)
    {
        m_scale = scale;
    }


private: // data menber
    Model m_shadowModel;

    bool m_finishInit = false; // ‰Šú‰»‚ªI‚í‚Á‚½‚©

    Vector3 m_position = g_vec3Zero;			// ˆÊ’u
    Quaternion m_rotation = g_quatIdentity;		// ‰ñ“]
    Vector3 m_scale = g_vec3One;				// Šg‘å

};