#ifndef CYBER_NODE_READER_H_
#define CYBER_NODE_READER_H_

#include <algorithm>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "cyber/proto/topology_change.pb.h"

#include "cyber/blocker/blocker.h"
#include "cyber/common/global_data.h"
#include "cyber/croutine/routine_factory.h"
#include "cyber/data/data_visitor.h"
#include "cyber/node/reader_base.h"
#include "cyber/scheduler/scheduler_factory.h"
#include "cyber/service_discovery/topology_manager.h"
#include "cyber/time/time.h"
#include "cyber/transport/transport.h"

namespace apollo {
namespace cyber {

template <typename M0>
using CallbackFunc = std::function<void(const std::shared_ptr<M0>&)>;

using proto::RoleType;

const uint32_t DEFAULT_PENDING_QUEUE_SIZE = 1;//默认等待处理的消息队列大小

/**
 * @class Reader
 * @brief Reader订阅一个通道，它有两个主要功能：
 * 1. 您可以传递一个`CallbackFunc`来处理消息一到达时
 * 2. 您可以观察Blocker缓存的消息。Reader会自动将消息推送到Blocker的`PublishQueue`，
 * 我们可以使用`Observe`从`PublishQueue`中获取消息到`ObserveQueue`。
 * 但是，如果您设置了CallbackFunc，则可以忽略此功能。一个Reader使用一个`ChannelBuffer`，
 * 我们处理的消息存储在ChannelBuffer中。Reader将在初始化时加入拓扑，并在关闭时离开拓扑。
 * @warning 为了节省资源，`ChannelBuffer`的长度有限，通过`pending_queue_size`参数传递。
 * pending_queue_size默认设置为1，因此，如果您的处理速度比写入速度慢，则未处理的旧消息将丢失。
 * 您可以增加`pending_queue_size`来解决此问题。
 */
template <typename MessageT>
class Reader : public ReaderBase {
 public:
  using BlockerPtr = std::unique_ptr<blocker::Blocker<MessaeT>>;
  using ReceiverPtr = std::shared_ptr<transport::Receiver<MessageT>>;
  using ChangeConnectiong = typename service_discovery::Manager::ChangeConnection;
  using Iterator = typename std::list<std::shared_ptr<MessageT>>::const_iterator;

  /**
   * Constructor a Reader object.
   * @param role_attr is a protobuf message RoleAttributes, which includes the
   * channel name and other info.
   * @param reader_func is the callback function, when the message is received.
   * @param pending_queue_size is the max depth of message cache queue.
   * @warning the received messages is enqueue a queue,the queue's depth is
   * pending_queue_size
   */ 
  explicit Reader(const proto::RoleAttributes& role_attr,
                  const CallbackFunc<MessageT>& reader_func = nullptr,
                  uint32_t pending_queue_size = DEFAULT_PENDING_QUEUE_SIZE);
  virtual ~Reader();

  /**
   * @brief Init Reader
   *
   * @return true if init successfully
   * @return false if init failed
   */
  bool Init() override;


  /**
   * @brief Shutdown Reader
   */
  void Shutdown() override;

  /**
   * @brief Get All data that `Blocker` stores
   */
  void Observe() override;

  /**
   * @brief Clear `Blocker`'s data
   */
  void ClearData() override;

  /**
   * @brief Query whether we have received data since last clear
   *
   * @return true if the reader has received data
   * @return false if the reader has not received data
   */
  bool HasReceived() const override;

  /**
   * @brief Query whether the Reader has data to be handled
   *
   * @return true if blocker is empty
   * @return false if blocker has data
   */
  bool Empty() const override;

  /**
   * @brief Get time interval of since last receive message
   *
   * @return double seconds delay
   */
  double GetDelaySec() const override;

  /**
   * @brief Get pending_queue_size configuration
   *
   * @return uint32_t the value of pending queue size
   */
  uint32_t PendingQueueSize() const override;

  /**
   * @brief Push `msg` to Blocker's `PublishQueue`
   *
   * @param msg message ptr to be pushed
   */
  virtual void Enqueue(const std::shared_ptr<MessageT>& msg);

  /**
   * @brief Set Blocker's `PublishQueue`'s capacity to `depth`
   *
   * @param depth the value you  want to set
   */
  virtual void SetHistoryDepth(const uint32_t& depth);

  /**
   * @brief Get Blocker's `PublishQueue`'s capacity
   *
   * @return uint32_t depth of the history
   */
  virtual uint32_t GetHistoryDepth() const;

  /**
   * @brief Get the latest message we `Observe`
   *
   * @return std::shared_ptr<MessageT> the latest message
   */
  virtual std::shared_ptr<MessageT> GetLatestObserved() const;

  /**
   * @brief Get the oldest message we `Observe`
   *
   * @return std::shared_ptr<MessageT> the oldest message
   */
  virtual std::shared_ptr<MessageT> GetOldestObserved() const;

  /**
   * @brief Get the begin iterator of `ObserveQueue`, used to traverse
   *
   * @return Iterator begin iterator
   */
  virtual Iterator Begin() const { return blocker_->ObservedBegin(); }

  /**
   * @brief Get the end iterator of `ObserveQueue`, used to traverse
   *
   * @return Iterator begin iterator
   */
  virtual Iterator End() const { return blocker_->ObservedEnd(); }

  /**
   * @brief Is there is at least one writer publish the channel that we
   * subscribes?
   *
   * @return true if the channel has writer
   * @return false if the channel has no writer
   */
  bool HasWriter() override;

  /**
   * @brief Get all writers pushlish the channel we subscribes
   *
   * @param writers result vector of RoleAttributes
   */
  void GetWriters(std::vector<proto::RoleAttributes>* writers) override;

 protected:
  double latest_recv_time_sec_ = -1.0;
  double second_to_lastest_recv_time_sec_ = -1.0;
  uint32_t pending_queue_size_;

 private:
  void JoinTheTopology();
  void LeaveTheTopology();
  void OnChannelChange(const proto::ChangeMsg& changes_msg);
  
  CallbackFunc<MessageT> reader_func_;
  ReceiverPtr receiver_ = nullptr;
  std::string croutine_name_;

  BlockerPtr blocker_ = nullptr;

  ChangeConnection change_conn_;
  service_discovery::ChannelManagerPtr channel_manager_ = nullptr;
};


}
}