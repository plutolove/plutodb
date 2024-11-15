#pragma once
#include <memory>

#include "Common/exception.h"

namespace pluto {

enum class QueryNodeType : uint8_t {
  SELECT_NODE = 1,
  SET_OPERATION_NODE = 2,
  BOUND_SUBQUERY_NODE = 3,
  RECURSIVE_CTE_NODE = 4,
  CTE_NODE = 5
};

class QueryNode {
 public:
  explicit QueryNode(QueryNodeType type) : type(type) {}
  virtual ~QueryNode() {}

  QueryNodeType type;

 public:
  virtual std::string ToString() const = 0;

  // virtual bool Equals(const QueryNode *other) const;

  virtual std::unique_ptr<QueryNode> Copy() const = 0;

  // std::string ResultModifiersToString() const;

  void AddDistinct();

 protected:
  // void CopyProperties(QueryNode &other) const;

 public:
  template <class TARGET>
  TARGET &Cast() {
    if (type != TARGET::TYPE) {
    }
    return reinterpret_cast<TARGET &>(*this);
  }

  template <class TARGET>
  const TARGET &Cast() const {
    if (type != TARGET::TYPE) {
    }
    return reinterpret_cast<const TARGET &>(*this);
  }
};
}  // namespace pluto
