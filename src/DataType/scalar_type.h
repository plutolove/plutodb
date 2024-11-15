#pragma once

#include <string_view>

#include "Common/type_name.h"
#include "data_type.h"

namespace pluto {

template <typename T>
class ScalarType : public DataTypeImpl<ScalarType<T>> {
 public:
  using Inner = T;
  ScalarType() : DataTypeImpl<ScalarType<T>>() {}
};

}  // namespace pluto
