#ifndef CYBER_COMPONENT_COMPONENT_BASE_H
#define CYBER_COMPONENT_COMPONENT_BASE_H

#include <atomic>
#include <memory>
#include <string>
#include <vector>
#include "gflags/gflags.h"

#include "cyber/proto/component_conf.pb.h"

#include "cyber/class_loader/class_loader.h"
#include "cyber/common/environment.h"
#include "cyber/common/file.h"
#include "cyber/node/node.h"
#include "cyber/scheduler/scheduler.h"

namespace apollo {
namespace cyber {
/*
* 如果一个类继承自 std::enable_shared_from_this<T>，那么这个类的对象可以通过成员函数 
* shared_from_this() 返回一个指向自身的 std::shared_ptr 对象，而不会造成悬空指针问题（dangling pointer）。
*/
class ComponentBase : public std::enable_shared_from_this<ComponentBase>{
  public:
    template <typename M>
    using Reader = cyber::Reader<M>;

    virtual ~ComponentBase() {}

    virtual bool Initialize(const Componentconfig& config) { return false; }
    virtual bool Initialize(const TimerComponentConfig& config) { return false; }
    virtual void Shutdown() {
      if(is_shutdown_.exchange(true)) {
        return;
      }

      Clear();
      for(auto& reader : reader_) {
        reader->Shutdown();
      }
      scheduler::Instance()->RemoveTask(node_->Name())
    }



  std::atomic<bool> is_shutdown_ = {false};
  std::shared_ptr<Node> node_ = nullptr;
  std::string config_file_path_ = "";
  std::vector<std::shared_ptr<ReaderBase>> readers_;
};
}
}
 


#endif