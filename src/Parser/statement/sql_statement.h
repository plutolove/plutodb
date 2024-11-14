#pragma once
#include "Parser/enums/statement_type.h"

namespace pluto {

class SQLStatement {
 public:
  static constexpr const StatementType TYPE = StatementType::INVALID_STATEMENT;

  SQLStatement(StatementType type) : type(type) {}
  virtual ~SQLStatement() {}

  StatementType type;
};

}  // namespace pluto
