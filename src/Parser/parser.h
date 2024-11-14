#pragma once
#include <string>

#include "Common/maybe.h"

namespace pluto {

struct ParserOptions {
  bool preserve_identifier_case = true;
};

class Parser {
 public:
  Parser(ParserOptions options = ParserOptions{}) : options_(options) {}

  Maybe<std::string> parse(const std::string& sql);

 private:
  ParserOptions options_;
};

}  // namespace pluto
