#include <iostream>

#include "Common/auto_registration_factory.h"
#include "Common/exception.h"
#include "Common/log.h"
#include "Common/maybe.h"
#include "Common/type_name.h"
#include "Common/type_trait.h"
#include "Parser/parser.h"
#include "fmt/format.h"
#include "postgres_parser.hpp"
#include "tabulate/table.hpp"

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
  return pluto::Maybe<void>::Ok();
}

int main(int argc, char** argv) {
  LOG_INFO("--------------");
  pluto::Parser parser;
  parser.parse("select a, b, c from g;");
  auto* ptr = GET_CLASS(Base, test);
  // auto* ptr1 = GET_CLASS_OR_THROW(Base, test1);
  tabulate::Table t;
  t.add_row({"abbbbbbbb", "bcccccccc", "cdddddd"});
  t.add_row({"1", "23245456562345", "32343454565656hhhhh"});
  t.print(std::cout);
  t.column(0).format().width(3).multi_byte_characters(true);
  t.column(1).format().multi_byte_characters(true);
  std::cout << std::endl;
  LOG_INFO("typename: {}", pluto::TypeName<std::vector<std::string>>());
  return 0;
}
