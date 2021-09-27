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
    




    //static void CreateShadowMap();


public: //Get関数
 /*   static RenderTarget& GetShadowMap()
    {
        return m_shadowMap;
    }*/

    /**
     * @brief 位置を設定
     * @param vec 位置
    */
    void SetPosition(const Vector3& position)
    {
        m_position = position;
    }


private: //data menber
    Model m_shadowModel;

    //static RenderTarget m_shadowMap;

    bool m_finishInit = false; //初期化が終わったか

    Vector3 m_position = g_vec3Zero;			//位置
    Quaternion m_rotation = g_quatIdentity;		//回転
    Vector3 m_scale = g_vec3One;				//拡大

};