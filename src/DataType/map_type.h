#pragma once
#include "Common/type_name.h"
#include "DataType/data_type.h"

namespace pluto {

template <template <typename...> class M, typename K, typename V>
class MapType : public DataTypeImpl<MapType<M, K, V>> {
 public:
  using Inner = M<K, V>;
  MapType() : DataTypeImpl<MapType<M, K, V>>(), key_type(), value_type() {}

  DataTypePtr key_type;
  DataTypePtr value_type;
};

}  // namespace pluto
