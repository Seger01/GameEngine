#ifndef TYPE_ID_GENERATOR_H
#define TYPE_ID_GENERATOR_H

#include <cstdint>
#include <type_traits>

// TypeID generator using templates
template <typename T> class NetworkTypeIDGenerator {
public:
    static uint32_t GetID() {
        static const uint32_t id = GenerateID();
        return id;
    }

private:
    static constexpr uint32_t GenerateID() {
        // Generate a unique ID based on the type
        return static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&idStorage));
    }

    static constexpr int idStorage = 0; // Unique storage for each type
};

#define TYPE_ID(Type) NetworkTypeIDGenerator<Type>::GetID()

#endif // TYPE_ID_GENERATOR_H
