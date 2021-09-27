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


public: //Get�֐�
 /*   static RenderTarget& GetShadowMap()
    {
        return m_shadowMap;
    }*/

    /**
     * @brief �ʒu��ݒ�
     * @param vec �ʒu
    */
    void SetPosition(const Vector3& position)
    {
        m_position = position;
    }


private: //data menber
    Model m_shadowModel;

    //static RenderTarget m_shadowMap;

    bool m_finishInit = false; //���������I�������

    Vector3 m_position = g_vec3Zero;			//�ʒu
    Quaternion m_rotation = g_quatIdentity;		//��]
    Vector3 m_scale = g_vec3One;				//�g��

};