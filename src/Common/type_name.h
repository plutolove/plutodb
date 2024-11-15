#pragma once
#include <string>

namespace pluto {

template <typename T>
inline constexpr std::string_view TypeName = "none";

#define REG_TYPE_NAME(name, ...) \
  template <>                    \
  inline constexpr std::string_view Typename<__VA_ARGS__> = name;

}  // namespace pluto
