#pragma once
#include <memory>
#include <string>
#include <unordered_map>

namespace pluto {

template <typename Base, typename K = std::string>
class AutoRegistrationFactory {
 public:
  template <typename Derived>
  struct RawRegisterType {
    RawRegisterType(K&& key) {
      AutoRegistrationFactory::Get().getMutableObjs()->emplace(
          key, std::make_shared<Derived>());
    }
  };

  static AutoRegistrationFactory& Get() {
    static AutoRegistrationFactory instance_;
    return instance_;
  }

 private:
  bool has_objs() const { return objs_.get() != nullptr; }
  const auto& getObjs() const { return objs_; }
  auto* getMutableObjs() {
    if (not objs_)
      objs_.reset(new std::unordered_map<std::string, std::shared_ptr<Base>>());
    return objs_.get();
  }
  std::unique_ptr<std::unordered_map<std::string, std::shared_ptr<Base>>> objs_;
};

#define INTERNAL_CAT(a, b) a##b

#define PP_CAT(a, b) INTERNAL_CAT(a, b)

#define REGISTER_VAR_NAME PP_CAT(g_registry_var, __COUNTER__)

#define REGISTER_CLASS(Base, Derived)                            \
  static AutoRegistrationFactory<Base>::RawRegisterType<Derived> \
  REGISTER_VAR_NAME(k)

}  // namespace pluto
