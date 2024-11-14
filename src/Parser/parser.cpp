#include "Parser/parser.h"

#include "Common/maybe.h"
#include "postgres_parser.hpp"

namespace pluto {

Maybe<std::string> Parser::parse(const std::string& sql) {
  duckdb::PostgresParser parser;
  parser.Parse(sql);
  OK_OR_RETURN(parser.success,
               fmt::format("{}, location: {}", parser.error_message,
                           parser.error_location));
  return "------";
}

}  // namespace pluto
