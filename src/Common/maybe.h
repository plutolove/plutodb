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

  Error&& addFrame(const std::string& file, uint64_t line,
                   const std::string& func, const std::string& err) {
    error_frames_.emplace_back(file, line, func, err);
    return std::move(*this);
  }

 private:
  std::vector<ErrorFrame> error_frames_;
};

template <typename T, typename = guard::Guard>
class Maybe;

template <typename T>
class Maybe<T,
            guard::ValueGuard<not(IsScalar_v<T> || std::is_same_v<T, void>) &&
                              not std::is_reference_v<T>>> {};

template <typename T>
class Maybe<T, guard::ValueGuard<IsScalar_v<T> && not std::is_same_v<T, void> &&
                                 not std::is_reference_v<T>>> {};

template <typename T>
class Maybe<T, guard::ValueGuard<std::is_same_v<T, void>>> {};

template <typename T>
class Maybe<T,
            guard::ValueGuard<not(IsScalar_v<T> || std::is_same_v<T, void>) &&
                              std::is_reference_v<T>>> {};

}  // namespace pluto
