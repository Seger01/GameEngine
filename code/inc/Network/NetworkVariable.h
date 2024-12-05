#ifndef NETWORKVARIABLE_H
#define NETWORKVARIABLE_H

#include "Network/INetworkBehaviour.h"
#include "Network/NetworkInformation.h"

#include <stdexcept>

class NetworkVariableBase
{
public:
	NetworkVariableBase() : mDirty(false), mNetworkVariableID(-1) {}

	virtual ~NetworkVariableBase() = default;

	virtual void serialize(SLNet::BitStream& stream) const = 0;
	virtual void deserialize(SLNet::BitStream& stream) = 0;

	bool isDirty() const { return mDirty; }

	void setClean() { mDirty = false; }

	virtual uint32_t getTypeId() const = 0;

	void setNetworkVariableID(int aNetworkVariableID) { mNetworkVariableID = aNetworkVariableID; }

	int getNetworkVariableID() const { return mNetworkVariableID; }

protected:
	bool mDirty;
	int mNetworkVariableID;
};

template <typename INetworkSerializableTemplate> class NetworkVariable : public NetworkVariableBase
{
public:
	// NetworkVariable(WritePermission aWritePermission);
	NetworkVariable(INetworkBehaviour* aOwner)
	{
		if (aOwner)
		{
			aOwner->RegisterNetworkVariable(this);
		}
		else
		{
			throw std::runtime_error("NetworkVariable::NetworkVariable() aOwner is nullptr");
		}
	}

	INetworkSerializableTemplate& getValue() { return mValue; }

	void setValue(INetworkSerializableTemplate aValue)
	{
		if (mValue != aValue)
		{
			mValue = aValue;
			mDirty = true;
		}
	}

	void serialize(SLNet::BitStream& stream) const override { mValue.serialize(stream); }

	void deserialize(SLNet::BitStream& stream) override { mValue.deserialize(stream); }

	uint32_t getTypeId() const override { return GetTypeId<INetworkSerializableTemplate>(); }

private:
	INetworkSerializableTemplate mValue;
};

#endif // NETWORKVARIABLE_H