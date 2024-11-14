#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "Common/log.h"

namespace pluto {

template <typename Base, typename K = std::string>
class AutoRegistrationFactory {
 public:
  template <typename Derived>
  struct RawRegisterType {
    RawRegisterType(const K& key) {
      LOG_INFO("register obj: {}", key);
      auto& instance = AutoRegistrationFactory::Get();
      if (instance.hasRegist(key)) {
        LOG_ERROR("has registed key: {}", key);
      }
      instance.getMutableObjs()->emplace(key, std::make_unique<Derived>());
    }
  };

  static AutoRegistrationFactory& Get() {
    static AutoRegistrationFactory instance_;
    return instance_;
  }

  Base* getObj(const K& key) const {
    auto iter = objs_->find(key);
    if (iter != objs_->end()) {
      return iter->second.get();
    }
    return nullptr;
  }

 private:
  bool has_objs() const { return objs_.get() != nullptr; }
  const auto& getObjs() const { return objs_; }

  bool hasRegist(const K& key) {
    auto iter = objs_->find(key);
    return iter != objs_->end();
  }

  auto* getMutableObjs() {
    if (not objs_)
      objs_.reset(new std::unordered_map<std::string, std::unique_ptr<Base>>());
    return objs_.get();
  }

  std::unique_ptr<std::unordered_map<std::string, std::unique_ptr<Base>>> objs_;
};

#define INTERNAL_CAT(a, b) a##b

#define PP_CAT(a, b) INTERNAL_CAT(a, b)

#define REGISTER_VAR_NAME PP_CAT(__g_registry_var__, __COUNTER__)

#define REGISTER_CLASS(Base, key, Derived)                              \
  static pluto::AutoRegistrationFactory<Base>::RawRegisterType<Derived> \
      REGISTER_VAR_NAME(#key)

#define GET_CLASS(Base, key) \
  pluto::AutoRegistrationFactory<Base>::Get().getObj(#key)

}  // namespace pluto
