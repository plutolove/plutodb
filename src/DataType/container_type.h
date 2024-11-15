#pragma once
#include "Common/type_name.h"
#include "DataType/data_type.h"

namespace pluto {

template <template <typename...> class C, typename E>
class ContainerType : public DataTypeImpl<ContainerType<C, E>> {
 public:
  using Inner = C<E>;
  ContainerType() : DataTypeImpl<ContainerType<C, E>>(), ele_type() {}

  DataTypePtr ele_type;
};

}  // namespace pluto
