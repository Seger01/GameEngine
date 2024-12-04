#pragma once

#include "Network/INetworkSerializable.h"
#include "Transform.h"

class EnemyTransformSerialize : public INetworkSerializable
{
public:
	bool operator==(const EnemyTransformSerialize& other) const
	{
		if (mTransform.position.x == other.mTransform.position.x &&
			mTransform.position.y == other.mTransform.position.y && mTransform.rotation == other.mTransform.rotation &&
			mTransform.scale.x == other.mTransform.scale.x && mTransform.scale.y == other.mTransform.scale.y)
		{
			return true;
		}
		return false;
	}

	bool operator!=(const EnemyTransformSerialize& other) const { return !(*this == other); }

	void serialize(SLNet::BitStream& stream) const override
	{
		stream.Write(mTransform.position.x);
		stream.Write(mTransform.position.y);
		stream.Write(mTransform.rotation);
		stream.Write(mTransform.scale.x);
		stream.Write(mTransform.scale.y);
	}

	void deserialize(SLNet::BitStream& stream) override
	{
		stream.Read(mTransform.position.x);
		stream.Read(mTransform.position.y);
		stream.Read(mTransform.rotation);
		stream.Read(mTransform.scale.x);
		stream.Read(mTransform.scale.y);
	}

	Transform getTransform() const { return mTransform; }

	void setTransform(const Transform& aTransform) { mTransform = aTransform; }

private:
	Transform mTransform;
};
