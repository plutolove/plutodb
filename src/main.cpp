#include <iostream>

#include "Common/auto_registration_factory.h"
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

struct Base {
  virtual ~Base() {}
};

struct Derived : public Base {};

REGISTER_CLASS(Base, test, Derived);

pluto::Maybe<void> test_() {
  pluto::Parser parser;
  auto val =
      (({
         auto&& just_value = parser.parse("select a, b, c from g;");
         if (!pluto ::JustIsOk(just_value)) {
           return pluto ::AddErrFrame(
               just_value.error(),
               "/Users/bytedance/workspace/plutodb/src/main.cpp", __FUNCTION__,
               27, "parser.parse(\"select a, b, c from g;\")");
         }
         std ::forward<decltype(just_value)>(just_value);
       }).data());
}

int main(int argc, char** argv) {
  LOG_INFO("--------------");
  pluto::Parser parser;
  parser.parse("select a, b, c from g;");
  auto* ptr = GET_CLASS(Base, test);
  // auto* ptr1 = GET_CLASS_OR_THROW(Base, test1);
  return 0;
}
