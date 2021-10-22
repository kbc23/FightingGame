#pragma once
#include "physics_object_base.h"

class GhostObject : public PhysicsObjectBase
{
public:
	~GhostObject()
	{
		Release();
	}
	/*!
	* @brief	�S�[�X�g�I�u�W�F�N�g������B
	*@detail
	* �����I�ȃ^�C�~���O�ŃS�[�X�g�I�u�W�F�N�g���폜�������ꍇ�ɌĂяo���Ă��������B
	*/
	void Release() override final;
	/*!
	* @brief	�����œn���ꂽ�S�[�X�g�I�u�W�F�N�g���������g���ǂ�������B
	*/
	bool IsSelf(const btCollisionObject& ghost) const
	{
		return &ghost == &m_ghostObject;
	}
	/*!
	* @brief	���W��ݒ�B
	*/
	void SetPosition(const Vector3& pos)
	{
		auto& btTrans = m_ghostObject.getWorldTransform();
		btVector3 btPos;
		pos.CopyTo(btPos);
		btTrans.setOrigin(btPos);
	}
	/*!
	* @brief	��]��ݒ�B
	*/
	void SetRotation(const Quaternion& rot)
	{
		auto& btTrans = m_ghostObject.getWorldTransform();
		btQuaternion btRot;
		rot.CopyTo(btRot);
		btTrans.setRotation(btRot);
	}

	void setIgnoreCollisionCheck(const btCollisionObject* co, bool ignoreCollisionCheck) {
		m_ghostObject.setIgnoreCollisionCheck(co, ignoreCollisionCheck);
	}

	btGhostObject* GetGhostObject() { return &m_ghostObject; }

	bool IsRegistPhysicsWorld() {
		return m_isRegistPhysicsWorld;
	}


private:
	/*!
	* @brief	�S�[�X�g�쐬�����̋��ʏ����B
	*/
	void CreateCommon(Vector3 pos, Quaternion rot) override;



private:
	bool						m_isRegistPhysicsWorld = false;	//!<�������[���h�ɓo�^���Ă��邩�ǂ����̃t���O�B
	btGhostObject				m_ghostObject;	//!<�S�[�X�g



private: // data member



};