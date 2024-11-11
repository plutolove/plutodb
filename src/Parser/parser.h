#pragma once
#include "Common/maybe.h"
#include "postgres_parser.hpp"

namespace pluto {

struct ParserOptions {
  bool preserve_identifier_case = true;
};

class Parser {
 public:
  Parser(ParserOptions options = ParserOptions{}) : options_(options) {}

  Maybe<void> parse(const std::string& sql);

 private:
  ParserOptions options_;
};

}  // namespace pluto
