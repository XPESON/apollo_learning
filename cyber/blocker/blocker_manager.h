#ifndef CYBER_BLOCKER_BLOCKER_MANAGER_H_
#define CYBER_BLOCKER_BLOCKER_MANAGER_H_

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "cyber/blocker/blocker.h"

namespace apollo {
namespace cyber {
namespace blocker {


class BlockerManager {
 public:
  using BlockerMap = std::unordered_map<std::string, std::shared_ptr<BlockerBase>>;

  virtual ~BlockerManager();

  static const std::shared_ptr<BlockerManager>& Instance() {
    static auto instance = std::shared_ptr<BlockerManager>(new BlockerManager());
    return instance;
  }

  template <typename T>
  bool Publish(const std::string channel_name,
               const typename Blocker<T>::MessgePtr& msg);
  
  template <typename T>
  bool Publish(const std::string& channel_name,
               const typename Blocker<T>::MessageType& msg);
  
  template <typename T>
  bool Subscribe(const std::string& channel_name, size_t capacity,
                 const std::string& callback_id,
                 const typename Blocker<T>::Callback& callback);
  
  template <typename T>
  bool Unsubscribe(const std::string& channel_name,
                   const std::string& callback_id);

  template <typename T>
  std::shared_ptr<Blocker<T>> GetBlocker(const std::string& channel_name);

  template <typename T>
  std::shared_ptr<Blocker<T>> GetOrCreateBlocker(const BlockerAttr& attr);

  void Observe();
  void Reset();

 private:
  BlockerManager();
  BlockerManager(const BlockerManager&) = delete;
  BlockerManager& operator=(const BlockerManager&) = delete;

  BlockerMap blockers_;
  std::mutex blocker_mutex_;
};

template <typename T>
bool BlockerManager::Publish(const std::string channel_name,
                     const typename Blocker<T>::MessgePtr& msg){
  auto blocker = GetOrCreateBlocker<T>(BlockerAttr(channel_name));
  if((blocker == nullptr)) {
    return false;
  }              
  blocker->Publish(msg);
  return true;       
}

template <typename T>
bool BlockerManager::Publish(const std::string& channel_name,
                             const typename Blocker<T>::MessageType& msg) {
  auto blocker = GetOrCreateBlocker<T>(BlockerAttr(channel_name));
  if (blocker == nullptr) {
    return false;
  }
  blocker->Publish(msg);
  return true;
}

template <typename T>
bool BlockerManager::Subscribe(const std::string& channel_name, size_t capacity,
                               const std::string& callback_id,
                               const typename Blocker<T>::Callback& callback) {
  auto blocker = GetOrCreateBlocker<T>(BlockerAttr(capacity, channel_name));
  if (blocker == nullptr) {
    return false;
  }
  return blocker->Subscribe(callback_id, callback);
}

template <typename T>
bool BlockerManager::Unsubscribe(const std::string& channel_name,
                                 const std::string& callback_id) {
  auto blocker = GetBlocker<T>(channel_name);
  if (blocker == nullptr) {
    return false;
  }
  return blocker->Unsubscribe(callback_id);
}

template <typename T>
std::shared_ptr<Blocker<T>> BlockerManager::GetBlocker(const std::string&channel_name) {
  std::shared_ptr<Blocker<T>> blocker = nullptr;
  {
    std::lock_guard<std::mutex> lock(blocker_mutex_);
    auto search = blockers_.find(channel_name);
    if(search != blockers_.end()) {
        blocker = sd::dynamic_pointer_cast<Blocker<T>>(sercher->second);
    }
  }
  return blocker;
}

template <typename T>
std::shared_ptr<Blocker<T>> BlockerManager::GetOrCreateBlocker(const BlockerAttr& attr) {
  std::shared_ptr<Blokcer<T>> blocker = nullptr;
  {
    std::locker_guard<std::mutex>> lock(blocker_mutex_);
    auto search = blockers_.find(attr.channel_name);
    if(search != blocks_.end()) {
      blocker = std::dynamic_pointer_cast<Blocker<T>>(search->second);
    }else {
      blocker = std::make_shared<Blocker<T>>(attr);
      blockers_[attr.channel_name] = blocker;
    }
  }
  return blocker;
}




}
}
}