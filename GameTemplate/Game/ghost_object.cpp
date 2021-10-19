/*!
 * @brief	�ÓI�I�u�W�F�N�g�B
 */


 //#include "tkEngine/tkEnginePreCompile.h"
#include "stdafx.h"
#include "ghost_object.h"

using namespace std;

void GhostObject::Release()
{
	if (m_isRegistPhysicsWorld == true) {
		PhysicsWorld::GetInstance()->RemoveCollisionObject(m_ghostObject);
		m_isRegistPhysicsWorld = false;
	}
}
void GhostObject::CreateCommon(Vector3 pos, Quaternion rot)
{
	m_ghostObject.setCollisionShape(m_collider->GetBody());
	btTransform btTrans;
	btTrans.setOrigin({ pos.x, pos.y, pos.z });
	btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });
	m_ghostObject.setWorldTransform(btTrans);

	//�����G���W���ɓo�^�B
	PhysicsWorld::GetInstance()->AddCollisionObject(m_ghostObject);
	m_isRegistPhysicsWorld = true;
}
