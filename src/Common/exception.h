#pragma once
#include <fmt/format.h>

#include "Common/exception_base.h"
#include "fmt/core.h"

namespace pluto {

class Exception : public ExceptionBase {
 public:
  Exception() = default;

  Exception(int code, const std::string &msg) : ExceptionBase(msg, code) {}

  template <typename... Args>
  Exception(int code, const std::string &fmt, Args &&...args)
      : ExceptionBase(
            fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...), code) {
  }

  template <typename... Args>
  Exception(const std::string &file, int64_t line,
            const std::string &function_name, int code, const std::string &fmt,
            Args &&...args)
      : ExceptionBase(
            fmt::format("{}:{}, function: {}", file, line, function_name),
            fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...), code) {
  }

  template <typename... Args>
  Exception(const std::string &file, int64_t line,
            const std::string &function_name, const char *fmt, Args &&...args)
      : ExceptionBase(
            fmt::format("{}:{}, function: {}", file, line, function_name),
            fmt::format(fmt::runtime(fmt), std::forward<Args>(args)...)) {}

  [[nodiscard]] Exception *clone() const override {
    return new Exception(*this);
  }

  void rethrow() const override { throw *this; }

  [[nodiscard]] const char *name() const noexcept override {
    return "pluto::Exception";
  }

  [[nodiscard]] const char *what() const noexcept override {
    return message().data();
  }

 private:
  [[nodiscard]] const char *className() const noexcept override {
    return "pluto::Exception";
  }
};

}  // namespace pluto

#define ThrowException(...) \
  throw pluto::Exception(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
