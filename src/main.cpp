#include <iostream>

#include "Common/log.h"
#include "Common/maybe.h"
#include "Common/type_trait.h"
#include "Parser/parser.h"
#include "fmt/format.h"
#include "postgres_parser.hpp"

int main(int argc, char** argv) {
  LOG_INFO("--------------");
  pluto::Parser parser;
  parser.parse("select a, b, c from g;");
  return 0;
}
