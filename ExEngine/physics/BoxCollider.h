#pragma once

#include "ICollider.h"

/*!
	* @brief	�{�b�N�X�R���C�_�[�B
	*/
class BoxCollider : public ICollider
{
public:
	// LNK2019���������邽�߃C�����C���֐���
	void Create(const Vector3& size)
	{
		shape = std::make_unique<btBoxShape>(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f));
	}

	btCollisionShape* GetBody() const override
	{
		return shape.get();
	}
private:
	std::unique_ptr<btBoxShape>		shape;
};
