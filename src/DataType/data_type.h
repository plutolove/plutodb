#pragma once
#include <cassert>
#include <string>

#include "Common/type_name.h"

namespace pluto {

class DataType {
 public:
  DataType() {}
  virtual ~DataType() {}
  virtual std::string typeName() const = 0;
};

using DataTypePtr = DataType*;

template <typename T>
class DataTypeImpl : public DataType {
 public:
  DataTypeImpl() : DataType() { assert(TypeName<typename T::Inner> != "none"); }

  virtual std::string typeName() const {
    return std::string(TypeName<typename T::Inner>);
  }
};

// template <template <typename...> class C, typename E>
// struct ContainerType : public DataType {
//   using Valuetype = C<E>;
//   ContainerType() : ele_type(TypeSystem.get(TypeName<E>), sizeof(Valuetype))
//   {} DataTypePtr ele_type; size_t size;
// };

// template <>
// struct Ref {};

// TypeUtil {
//   issclar;
//   isVec;
//   isxxx;
// }

// REG_Type(name, ContainerType<...>);

// op<TypeList1<>, TypeList2<> , typename F, template<typename...> class OP>

// type1, type2

}  // namespace pluto
