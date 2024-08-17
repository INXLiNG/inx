#ifndef __INX_TYPES_H__
#define __INX_TYPES_H__

#include <cstdint>
#include <memory>

typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;
typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;

namespace inx
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr Scope<T> create_scope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> create_ref(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
} // namespace inx

#endif // __INX_TYPES_H__