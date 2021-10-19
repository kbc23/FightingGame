/*!
* @brief	球体コライダー。
*/

#pragma once

#include "ICollider.h"

class SphereCollider : public ICollider
{
public:
	// LNK2019が発生するためインライン関数化
	void Create(const float radius)
	{
		m_shape = std::make_unique<btSphereShape>(radius);
	}

	btCollisionShape* GetBody() const override
	{
		return m_shape.get();
	}
private:
	std::unique_ptr<btSphereShape>	m_shape;
};
