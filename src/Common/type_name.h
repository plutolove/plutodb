#pragma once
#include <cxxabi.h>

#include <string>

namespace pluto {
namespace {

template <typename T>
std::string TypeNameImpl() {
  int status;
  std::string tname = typeid(T).name();
  char *demangled_name =
      abi::__cxa_demangle(tname.c_str(), NULL, NULL, &status);
  if (status == 0) {
    tname = demangled_name;
    std::free(demangled_name);
  }
  return tname;
}

}  // namespace

template <typename T>
inline std::string_view TypeName() {
  static auto type_name = TypeNameImpl<T>();
  return type_name;
}

}  // namespace pluto
