#pragma once
#include <memory>
#include <shared_mutex>
#include <type_traits>
#include <variant>

#include "Common/exception.h"
#include "Common/log.h"
#include "Common/type_trait.h"
#include "fmt/format.h"

class ErrorFrame final {
 public:
  ErrorFrame(const std::string& f, const std::string& func, size_t l,
             const std::string& err)
      : file(f), function_name(func), line(l), err_msg(err) {}

  std::string debugString() const {
    return fmt::format("{}:{} {}:\n\t{}\n", file, line, function_name, err_msg);
  }

 private:
  std::string file;
  std::string function_name;
  size_t line;
  std::string err_msg;
};

class Error final {
 public:
  Error() {}

  Error(const Error&) = default;
  Error(Error&&) = default;

  Error&& addErrorFrame(const std::string& file, const std::string& func,
                        size_t line, const std::string& err) {
    error_frames.emplace_back(file, func, line, err);
    return std::move(*this);
  }

  Error&& addErrorFrame(ErrorFrame&& err) {
    error_frames.push_back(std::move(err));
    return std::move(*this);
  }

  std::string debugString() const {
    std::string res;
    for (auto& frame : error_frames) {
      res.append(frame.debugString());
    }
    return res;
  }

  static Error CheckFailedError() { return Error(); }

 private:
  std::vector<ErrorFrame> error_frames;
};

using ErrorPtr = std::unique_ptr<Error>;

template <typename T, typename = type_trait::guard::Guard>
class Maybe;

template <typename T>
class Maybe<T, type_trait::guard::ValueGuard<not(type_trait::IsScalar_v<T> ||
                                                 std::is_same_v<T, void>) and
                                             not std::is_reference_v<T>>>
    final {
  using ValueT = std::unique_ptr<T>;

 public:
  Maybe(const T& data) : data_or_err_(std::make_unique<T>(data)) {}
  Maybe(T&& data) : data_or_err_(std::make_unique<T>(std::move(data))) {}

  Maybe(std::unique_ptr<T> ptr) : data_or_err_(std::move(ptr)) {}
  Maybe(const std::unique_ptr<T>& ptr) : data_or_err_(std::move(ptr)) {}

  Maybe(const Error& err) : data_or_err_(std::make_unique<Error>(err)) {}
  Maybe(Error&& err) : data_or_err_(std::make_unique<Error>(std::move(err))) {}

  Maybe(ErrorPtr err) : data_or_err_(std::move(err)) {}

  Maybe(Maybe&& data) : data_or_err_(std::move(data.data_or_err_)) {}

  Maybe(const Maybe&) = default;
  ~Maybe() = default;

  bool isOk() const { return std::holds_alternative<ValueT>(data_or_err_); }

  ValueT data() { return std::move(std::get<ValueT>(data_or_err_)); }

  ErrorPtr error() { return std::get<1>(data_or_err_); }

 private:
  std::variant<ValueT, ErrorPtr> data_or_err_;
};

template <typename T>
class Maybe<T, type_trait::guard::ValueGuard<std::is_same_v<T, void>>> final {
 public:
  Maybe(const Error& err) : data_or_err_(std::make_unique<Error>(err)) {}
  Maybe(Error&& err) : data_or_err_(std::make_unique<Error>(std::move(err))) {}

  Maybe(ErrorPtr err) : data_or_err_(std::move(err)) {}

  Maybe(Maybe&& data) : data_or_err_(std::move(data.data_or_err_)) {}

  Maybe(const Maybe&) = default;
  ~Maybe() = default;

  static Maybe Ok() { return Maybe(); }

  void data() const {}

  bool isOk() const { return std::holds_alternative<void*>(data_or_err_); }

  ErrorPtr error() { return std::move(std::get<ErrorPtr>(data_or_err_)); }

 private:
  Maybe() : data_or_err_(nullptr) {}
  std::variant<void*, ErrorPtr> data_or_err_;
};

template <typename T>
class Maybe<T, type_trait::guard::ValueGuard<type_trait::IsScalar_v<T> and
                                             not std::is_reference_v<T>>>
    final {
 public:
  Maybe(T data) : data_or_err_(data) {}

  Maybe(const Error& err) : data_or_err_(std::make_unique<Error>(err)) {}
  Maybe(Error&& err) : data_or_err_(std::make_unique<Error>(std::move(err))) {}

  Maybe(ErrorPtr err) : data_or_err_(std::move(err)) {}

  Maybe(Maybe&& data) = default;

  Maybe(const Maybe&) = default;
  ~Maybe() = default;

  bool isOk() const { return std::holds_alternative<T>(data_or_err_); }

  T data() const { return std::get<0>(data_or_err_); }

  ErrorPtr error() { return std::move(std::get<ErrorPtr>(data_or_err_)); }

 private:
  std::variant<T, ErrorPtr> data_or_err_;
};

template <typename T>
class Maybe<T, type_trait::guard::ValueGuard<std::is_reference_v<T>>> final {
  using ValueT = typename std::remove_reference_t<T>;
  using PtrT = ValueT*;

 public:
  Maybe(T data) : data_or_err_(&data) {}

  Maybe(const Error& err) : data_or_err_(std::make_unique<Error>(err)) {}
  Maybe(Error&& err) : data_or_err_(std::make_unique<Error>(std::move(err))) {}

  Maybe(ErrorPtr err) : data_or_err_(std::move(err)) {}

  Maybe(Maybe&& data) = default;
  Maybe(const Maybe&) = default;
  ~Maybe() = default;

  bool isOk() const { return std::holds_alternative<T>(data_or_err_); }

  const T& data() const { return *std::get<0>(data_or_err_); }
  T& data() { return *std::get<0>(data_or_err_); }

  ErrorPtr error() { return std::move(std::get<ErrorPtr>(data_or_err_)); }

 private:
  std::variant<PtrT, ErrorPtr> data_or_err_;
};

inline ErrorPtr&& AddErrFrame(ErrorPtr&& error, const std::string& file,
                              const std::string& func, size_t line,
                              const std::string& err) {
  error->addErrorFrame(file, func, line, fmt::format("Error Expr: {}", err));
  return std::move(error);
}

template <typename T>
inline bool JustIsOk(const Maybe<T>& val) {
  return val.isOk();
}

#define STRINGIZEIMP(x) #x
#define STRINGIZE(x) STRINGIZEIMP(x)

#define OK_OR_RETURN(expr)                                   \
  do {                                                       \
    if (not(expr)) {                                         \
      return Error::CheckFailedError().addErrorFrame(        \
          __FILE__, __FUNCTION__, __LINE__,                  \
          fmt::format("Check Failed: {}", STRINGIZE(expr))); \
    }                                                        \
  } while (0)

#define JUST_VAL_WRAPPER(...) __VA_ARGS__

#define JUST(...)                                                              \
  ({                                                                           \
    auto&& just_value = JUST_VAL_WRAPPER(__VA_ARGS__);                         \
    if (not JustIsOk(just_value)) {                                            \
      return AddErrFrame(just_value.error(), __FILE__, __FUNCTION__, __LINE__, \
                         STRINGIZE(__VA_ARGS__));                              \
    }                                                                          \
    std::forward<decltype(just_value)>(just_value);                            \
  }).data()

#define OK_OR_THROW(...)                                                  \
  do {                                                                    \
    auto&& just_value = JUST_VAL_WRAPPER(__VA_ARGS__);                    \
    if (not JustIsOk(just_value)) {                                       \
      auto err_ = AddErrFrame(just_value.error(), __FILE__, __FUNCTION__, \
                              __LINE__, STRINGIZE(__VA_ARGS__));          \
      ThrowException("{}", err_->debugString());                          \
    }                                                                     \
  } while (0)
