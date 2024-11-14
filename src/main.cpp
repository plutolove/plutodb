#include <iostream>

#include "Common/exception.h"
#include "Common/log.h"
#include "Common/maybe.h"
#include "Common/type_trait.h"
#include "Parser/parser.h"
#include "fmt/format.h"
#include "postgres_parser.hpp"

pluto::Maybe<int> test() {
  OK_OR_RETURN(234 < 34, "xxxx");
  return 324;
}

int main(int argc, char** argv) {
  LOG_INFO("--------------");
  pluto::Parser parser;
  parser.parse("select a, b, c from g;");
  return 0;
}
