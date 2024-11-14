#ifndef NETWORKVARIABLE_H
#define NETWORKVARIABLE_H

#include <stdexcept> // For std::runtime_error

enum class WritePermission
{
    ReadOnly,
    ReadWrite
};

template <typename T>
class NetworkVariable
{
public:
    NetworkVariable(WritePermission aWritePermission);
    T getValue() const;
    void setValue(T aValue);

private:
    T mValue;
};

template <typename T>
NetworkVariable<T>::NetworkVariable(WritePermission aWritePermission)
{
    throw std::runtime_error("NetworkVariable::NetworkVariable() not implemented");
}

template <typename T>
T NetworkVariable<T>::getValue() const
{
    throw std::runtime_error("NetworkVariable::getValue() not implemented");
}

template <typename T>
void NetworkVariable<T>::setValue(T aValue)
{
    throw std::runtime_error("NetworkVariable::setValue() not implemented");
}

#endif // NETWORKVARIABLE_H