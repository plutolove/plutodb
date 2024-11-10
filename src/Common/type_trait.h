#pragma once
#include <type_traits>

namespace pluto {

namespace guard {

using Guard = int;

template <bool V>
using ValueGuard = std::enable_if_t<V, Guard>;

template <typename T, typename... Ts>
using TypeGuard = std::enable_if_t<(std::is_same_v<T, Ts> || ...), Guard>;

}  // namespace guard

template <typename T, typename = guard::Guard>
struct IsScalar : std::false_type {};

template <typename T>
struct IsScalar<T,
                guard::ValueGuard<std::is_arithmetic_v<T> ||
                                  (sizeof(T) <= 16 &&
                                   std::is_trivially_copy_constructible_v<T> &&
                                   std::is_trivially_destructible_v<T>)>>
    : std::true_type {};

template <typename T>
inline constexpr bool IsScalar_v = IsScalar<T>::value;

}  // namespace pluto
