#ifndef CYBER_NODE_READER_BASE_H_
#define CYBER_NODE_READER_BASE_H_

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "cyber/common/macros.h"
#include "cyber/common/util.h"
#include "cyber/event/perf_event_cache.h"
#include "cyber/transport/transport.h"

namespace apollo {
namespace cyber {

using apollo::cyber::common::GlobalData;
using apollo::cyber::event::PerfEventCache;
using apollo::cyber::event::TransPerf;

/**
 * @class ReaderBase
 * @brief 读取器的基类
 * Reader 通过一个 apollo::cyber::proto::RoleAttribute 进行识别，
 * 它包含我们订阅的通道名称、通道ID，
 * 以及我们所在的主机名、进程ID和节点，
 * 以及描述我们传输质量的 QoS。
 */
class ReaderBase {
  public:
    explicit ReaderBase(const proto::RoleAtrributes& role_attr)
        : role_attr_(role_attr), init_(false) {}
    virtual ~ReaderBase() {}

    /**@brif Init the Reader object
     * 
     * @return true if init successfully
     * @return false if init failed
    */
    virtual bool Init() = 0;
    /**
     * @brief Shutdown the reader object
    */
    virtual void Shutdow() = 0;

    /**
        * @brief CLear local data
    */
    virtual void ClearData() = 0;

    /**
    * @brief Get store data
    */
    virtual void Observe() = 0;
    /**
        * @brief Query whether the Reader has data to be handle
        * 
        * @return true if data container is empty
        * @return false if data container has data
    */
    virtual bool Empty() const = 0;

    /**
     * @brief Query whether we have received data since last clear
     *
     * @return true if the reader has received data
     * @return false if the reader has not received data
     */
    virtual bool HasReceived() const = 0;

    /**
     * @brief Get time interval of since last recieve message
     * 
     * @return double second delay
    */
    virtual double GetDelaySec() const = 0;

    /**
     * @brief Get the value of pending queue size
     * 
     * @return uint32_t result value
    */
    virtual uint32_t PendingQueueSize() const = 0;

    /**
     * @brief Queue is there any writer that publish the subscribed channel
     * 
     * @return true if there is any write that publish the subscribed channel
     * @return false if there is no Writer publish the channel
    */
    virtual bool HasWriter() { return false; }

    /**
     * @brief Get all writers publish the channel we subscribes
     * 
     * @param writers result RoleAttributes vector
    */
    virtual void GetWriters(std::vector<proto::RoleAttributes>* writers) {}

    /**
     * @brief Get Reader's Channel name
     * 
     * @return const std::string& channel name
    */
    const std::string& GetChannelName() const {
      return role_attr_.channel_name();
    }

    /**
     * @brief Get Reader's Channel id
     * 
     * @return uint64_t channel id
    */
    uint64_t ChannelId() const { return role_attr_.channel_id(); }

    /**
     * @brief Get qos profile. You an see qos description
     * 
     * @return const proto::QosProfiles& result qos
    */
    const proto::QosProfile& QosProfile() const {
      return role_attr_.qos_profile();
    }

    /**
     * @brief Query whether the Reader is initialized
     * 
     * @return true if the Reader has been inited
     * @return false if the Reader as not been inited
    */
    bool IsInit() const { return init_.load(); }
    
  protected:
    proto::RoleAtrributes role_attr_;
    std::atomic<bool> init_;
};

/**
 * @brief 一个通道与一个接收器相关联。
 * ReceiverManager 负责将一个接收器附加到其相应的通道上。我们将一个 DataDispatcher 
 * 的回调函数传递给这个接收器，所以当接收到消息时，它将被推送到 `ChannelBuffer`，
 * 并且 `DataVisitor` 将 `Fetch` 数据并传递给 `Reader` 的回调函数。
 *
 * @tparam MessageT 消息类型。
 */
template <typename MessageT>
class ReceiverManager {
  public:
    ~ReceiverManager() { receiver_map_.clear(); }

    /**
     * @brief Get the Receiver object
     * 
     * @param role_attr the atrribute that the Receiver has
     * @return std::shared_ptr<transport::Receiver<MessageT>> result Receiver
     * 
     * 根据给定的 proto::RoleAttributes 参数返回一个 
     * std::shared_ptr<transport::Receiver<MessageT>> 类型的接收器指针。
    */
    auto GetReceiver(const proto::RoleAtrribute& role_attr) ->
        typename std::shared_ptr<transport::Receiver<MessageT>>;
    
  private:
    std::unordered_map<std::string, 
                       typename std::shared_ptr<transport::Receiver<MessageT>>>
        receiver_map_;
    std::mutex receiver_map_mutex_;

    DECLARE_SINGLETON(ReceiverManager<MessageT>)
};

}
}