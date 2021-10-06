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
    


public: //Get�֐�
    /**
     * @brief �ʒu��ݒ�
     * @param vec �ʒu
    */
    void SetPosition(const Vector3& position)
    {
        m_position = position;
    }

    /**
     * @brief ��]�ʂ�ݒ�
     * @param rotation ��]��
    */
    void SetRotation(const Quaternion& rotation)
    {
        m_rotation = rotation;
    }

    /**
     * @brief �g�嗦��ݒ�
     * @param scale �g�嗦
    */
    void SetScale(const Vector3& scale)
    {
        m_scale = scale;
    }


private: // data menber
    Model m_shadowModel;

    bool m_finishInit = false; // ���������I�������

    Vector3 m_position = g_vec3Zero;			// �ʒu
    Quaternion m_rotation = g_quatIdentity;		// ��]
    Vector3 m_scale = g_vec3One;				// �g��

};