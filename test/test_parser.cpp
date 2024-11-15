#include "Common/log.h"
#include "Common/maybe.h"
#include "gtest/gtest.h"
#include "postgres_parser.hpp"

using namespace pluto;

TEST(plutodb, lib_pg_query_parser) {
  duckdb::PostgresParser parser;
  std::string sql = R"EOF(select
supp_nation,
cust_nation,
l_year, sum(volume) as revenue 
from (
select
n1.n_name as supp_nation,
n2.n_name as cust_nation,
extract(year from l_shipdate) as l_year,
l_extendedprice * (1 - l_discount) as volume
from
supplier,lineitem,orders,customer,nation n1,nation n2 
where
s_suppkey = l_suppkey
and o_orderkey = l_orderkey
and c_custkey = o_custkey
and s_nationkey = n1.n_nationkey
and c_nationkey = n2.n_nationkey
and (
(n1.n_name = '[NATION1]' and n2.n_name = '[NATION2]')
or (n1.n_name = '[NATION2]' and n2.n_name = '[NATION1]')
)
and l_shipdate between date '1995-01-01' and date '1996-12-31'
) as shipping
group by
supp_nation,
cust_nation,
l_year
order by
supp_nation,
cust_nation,
l_year;)EOF";
  duckdb::PostgresParser::SetPreserveIdentifierCase(true);
  parser.Parse(sql);
  if (parser.success) {
    LOG_INFO("compile success");
  } else {
    LOG_ERROR("compile failed, {} {}", parser.error_message,
              parser.error_location);
  }
}

Maybe<int> maybe_test() {
  OK_OR_RETURN(23 < 2, "");
  return Maybe<int>(1);
}

Maybe<int> maybe_test_call() {
  auto data = JUST(maybe_test());
  OK_OR_THROW(maybe_test());
  return Maybe<int>(213);
}

TEST(maybe, maybe) {
  Maybe<void> tmp = Maybe<void>::Ok();
  Maybe<std::string_view> str_v("sdfgsdfg");
  std::vector<int> x{23, 34, 4356};
  Maybe<std::vector<int>> vec(x);
  std::variant<void*, std::shared_ptr<int>> var(nullptr);
  LOG_INFO("------ is void: {}", std::holds_alternative<void*>(var));
  int xx = 234;
  Maybe<int&> ref_int(xx);
  LOG_INFO("int val: {}", ref_int.data());
  LOG_INFO("str_view: {}", str_v.data());
  LOG_INFO("vec: [{}]", fmt::format("{}", fmt::join(*vec.data(), ", ")));
  auto val = maybe_test();
  auto val1 = maybe_test_call();
  if (not val.isOk()) {
    LOG_INFO("error: {}", val.error()->debugString());
  }
  if (not val1.isOk()) {
    LOG_INFO("value 1 error: {}", val1.error()->debugString());
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::string tmp = "sdfgsdfgsdg";
  pluto::Maybe<void> val = pluto::Maybe<void>::Ok();
  pluto::Maybe<int> int_val(23);
  pluto::Maybe<std::string> str_val("sdfgsdf");
  pluto::Maybe<std::string&> ref_val(tmp);
  return RUN_ALL_TESTS();
}

