#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "Common/exception.h"
#include "Common/log.h"

namespace pluto {

template <typename Base, typename K = std::string>
class AutoRegistrationFactory {
 public:
  AutoRegistrationFactory()
      : objs_map_(
            new std::unordered_map<std::string, std::unique_ptr<Base>>()) {}

  template <typename Derived>
  struct RawRegisterType {
    RawRegisterType(const K& key) {
      LOG_INFO("register obj: {}", key);
      auto& instance = AutoRegistrationFactory::Get();
      if (instance.hasRegist(key)) {
        LOG_ERROR("has registed key: {}", key);
      }
      instance.MutableObjsMap()->emplace(key, std::make_unique<Derived>());
    }
  };

  static AutoRegistrationFactory& Get() {
    static AutoRegistrationFactory instance_;
    return instance_;
  }

  Base* getObj(const K& key) const {
    if (not objs_map_) {
      LOG_ERROR("objs_ map is nullptr");
      return nullptr;
    }
    auto iter = objs_map_->find(key);
    if (iter != objs_map_->end()) {
      return iter->second.get();
    }
    LOG_ERROR("getObj failed, {} not found", key);
    return nullptr;
  }

  Base* getObjOrThrow(const K& key) const {
    if (not objs_map_) {
      ThrowException("objs_ map is nullptr");
    }
    auto iter = objs_map_->find(key);
    if (iter != objs_map_->end()) {
      return iter->second.get();
    }
    ThrowException("getObj failed, {} not found", key);
    return nullptr;
  }

 private:
  auto* MutableObjsMap() { return objs_map_.get(); }
  bool has_objs() const { return objs_map_.get() != nullptr; }

  bool hasRegist(const K& key) {
    auto iter = objs_map_->find(key);
    return iter != objs_map_->end();
  }

  std::unique_ptr<std::unordered_map<std::string, std::unique_ptr<Base>>>
      objs_map_;
};

#define INTERNAL_CAT(a, b) a##b

#define PP_CAT(a, b) INTERNAL_CAT(a, b)

#define REGISTER_VAR_NAME PP_CAT(__g_registry_var__, __COUNTER__)

#define REGISTER_CLASS(Base, key, Derived)                              \
  static pluto::AutoRegistrationFactory<Base>::RawRegisterType<Derived> \
      REGISTER_VAR_NAME(#key)

#define GET_CLASS(Base, key) \
  pluto::AutoRegistrationFactory<Base>::Get().getObj(#key)

#define GET_CLASS_OR_THROW(Base, key) \
  pluto::AutoRegistrationFactory<Base>::Get().getObjOrThrow(#key)

}  // namespace pluto
