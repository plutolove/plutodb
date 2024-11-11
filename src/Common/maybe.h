#pragma once
#include <cstdint>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

#include "Common/type_trait.h"
#include "fmt/format.h"

namespace pluto {

struct ErrorFrame {
  ErrorFrame(const std::string& file, uint64_t line, const std::string& func,
             const std::string& err)
      : file_(file), line_(line), function_(func), err_msg(err) {}

  std::string debugString() const {
    return fmt::format("{}:{} {}\n\t{}\n", file_, line_, function_, err_msg);
  }

  std::string file_;
  uint64_t line_;
  std::string function_;
  std::string err_msg;
};

class Error {
 public:
  Error() {}

  Error(const std::string& file, uint64_t line, const std::string& func,
        const std::string& err) {
    error_frames_.emplace_back(file, line, func, err);
  }

  void addFrame(const std::string& file, uint64_t line, const std::string& func,
                const std::string& err) {
    error_frames_.emplace_back(file, line, func, err);
  }

 private:
  std::vector<ErrorFrame> error_frames_;
};

using ErrorPtr = std::shared_ptr<Error>;

template <typename T>
inline ErrorPtr JustGetError() {
  return std::make_shared<Error>();
}

inline ErrorPtr&& JustErrorAddStackFrame(ErrorPtr&& err,
                                         const std::string& file, int64_t line,
                                         const std::string& func,
                                         const std::string& message) {
  err->addFrame(file, line, func, message);
  return std::move(err);
}

template <typename T, typename = guard::Guard>
class Maybe;

template <typename T>
class Maybe<T,
            guard::ValueGuard<not(IsScalar_v<T> || std::is_same_v<T, void>) &&
                              not std::is_reference_v<T>>> {
 public:
  using ValueType = std::shared_ptr<T>;
  Maybe(const T& data) : data_or_error_(std::make_shared<T>(data)) {}
  Maybe(T&& data) : data_or_error_(std::make_shared<T>(std::move(data))) {}

  Maybe(const std::shared_ptr<T>& data) : data_or_error_(data) {}
  Maybe(std::shared_ptr<T>&& data) : data_or_error_(std::move(data)) {}

  Maybe(const Error& error) : data_or_error_(std::make_shared<Error>(error)) {}
  Maybe(const std::shared_ptr<Error>& error) : data_or_error_(error) {}

  Maybe(const Maybe&) = default;
  Maybe(Maybe&& other) : data_or_error_(std::move(other.data_or_error_)) {}
  ~Maybe() = default;

  bool isOk() const {
    return std::holds_alternative<ValueType>(data_or_error_);
  }

  ValueType Data_UserCanNotUse() const { return std::get<0>(data_or_error_); }

  ErrorPtr stackError() const { return std::get<1>(data_or_error_); }

 private:
  std::variant<ValueType, ErrorPtr> data_or_error_;
};

template <typename T>
class Maybe<T, guard::ValueGuard<IsScalar_v<T> && not std::is_same_v<T, void> &&
                                 not std::is_reference_v<T>>> {
 public:
  Maybe(const T& data) : data_or_error_(data) {}
  Maybe(T&& data) : data_or_error_(std::move(data)) {}

  Maybe(const Error& error) : data_or_error_(std::make_shared<Error>(error)) {}
  Maybe(const std::shared_ptr<Error>& error) : data_or_error_(error) {}

  Maybe(const Maybe&) = default;
  Maybe(Maybe&& other) = default;
  ~Maybe() = default;

  bool isOk() const { return std::holds_alternative<T>(data_or_error_); }

  T Data_UserCanNotUse() const { return std::get<0>(data_or_error_); }

  ErrorPtr stackError() const { return std::get<1>(data_or_error_); }

 private:
  std::variant<T, ErrorPtr> data_or_error_;
};

template <typename T>
class Maybe<T, guard::ValueGuard<std::is_same_v<T, void>>> {
 public:
  static Maybe Ok() { return Maybe(); }
  Maybe(const Error& error) : data_or_error_(std::make_shared<Error>(error)) {}
  Maybe(const std::shared_ptr<Error>& error) : data_or_error_(error) {}

  Maybe(const Maybe&) = default;
  Maybe(Maybe&& other) = default;
  ~Maybe() = default;

  bool isOk() const { return std::holds_alternative<void*>(data_or_error_); }

  void* Data_UserCanNotUse() const { return nullptr; }

  ErrorPtr stackError() const { return std::get<1>(data_or_error_); }

 private:
  Maybe() : data_or_error_(nullptr) {}
  std::variant<void*, ErrorPtr> data_or_error_;
};

template <typename T>
class Maybe<T,
            guard::ValueGuard<not(IsScalar_v<T> || std::is_same_v<T, void>) &&
                              std::is_reference_v<T>>> {
 public:
  using ValueType = std::add_pointer_t<T>;
  Maybe(T data) : data_or_error_(&data) {}

  Maybe(const Error& error) : data_or_error_(std::make_shared<Error>(error)) {}
  Maybe(const std::shared_ptr<Error>& error) : data_or_error_(error) {}

  Maybe(const Maybe&) = default;
  Maybe(Maybe&& other) = default;
  ~Maybe() = default;

  bool isOk() const {
    return std::holds_alternative<ValueType>(data_or_error_);
  }

  T Data_UserCanNotUse() const { return *std::get<0>(data_or_error_); }

  ErrorPtr stackError() const { return std::get<1>(data_or_error_); }

 private:
  std::variant<ValueType, ErrorPtr> data_or_error_;
};

template <typename T>
bool JustIsOk(const Maybe<T>& val) {
  return val.isOk();
}

inline Error&& AddErrorFrame(Error&& err, const std::string& file,
                             uint64_t line, const std::string& func,
                             const std::string& msg) {
  err.addFrame(file, line, func, msg);
  return std::move(err);
}

#define NewError(msg) \
  std::make_shared<Error>(__FILE__, __LINE__, __FUNCTION__, msg);

#define OK_OR_RETURN(expr, msg) \
  do {                          \
    if (expr) {                 \
      return NewError(msg);     \
    }                           \
  } while (0);

#define JUST(...)                                                           \
  ({                                                                        \
    auto&& _just_value_ = (__VA_ARGS__);                                    \
    if (!JustIsOk(_just_value_)) {                                          \
      return AddErrorFrame(std::move(*_just_value_.stackError()), __FILE__, \
                           __LINE__, __FUNCTION__, #__VA_ARGS__);           \
    }                                                                       \
    std::forward<decltype(_just_value_)>(_just_value_);                     \
  }).Data_UserCanNotUse()

}  // namespace pluto
