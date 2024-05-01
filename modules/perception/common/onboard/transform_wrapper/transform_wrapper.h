#pragma once

#include <deque>
#include <memory>
#include <string>

#include "Eigen/Core"
#include "Eigen/Dense"
#include "gflags/gflags.h"// Google 开源的一个 C++ 库，用于解析和管理命令行参数。

#include "modules/transform/buffer.h"

namespace apollo {
namespace perception {
namespace onboard {

using apollo::transform::Buffer;
/*
这些声明语句使得这些全局变量在程序的其他部分可见，
并且可以通过调用 gflags 库提供的函数来访问这些变量的值。
这样做的好处是可以在运行程序时通过命令行参数来改变这些变量的值，而不需要修改源代码。
*/
//声明了一个名为 obs_sensor2novatel_tf2_frame_id 的字符串类型全局变量。
//这可能用于存储传感器观测数据到 NovAtel 导航系统的转换坐标系的字符串标识符。
DECLARE_string(obs_sensor2novatel_tf2_frame_id);
//声明了一个名为 obs_novatel2world_tf2_frame_id 的字符串类型全局变量。
//这可能用于存储 NovAtel 导航系统到世界坐标系的转换坐标系的字符串标识符。
DECLARE_string(obs_novatel2world_tf2_frame_id);
//声明了一个名为 obs_novatel2world_tf2_child_frame_id 的字符串类型全局变量。
//这可能用于存储 NovAtel 导航系统到世界坐标系的子坐标系的字符串标识符。
DECLARE_string(obs_novatel2world_tf2_child_frame_id);
//声明了一个名为 obs_tf2_buff_size 的双精度浮点型全局变量。这可能用于存储 TF2 缓冲区的大小。
DECLARE_double(obs_tf2_buff_size);
//声明了一个名为 hardware_trigger 的布尔型全局变量。这可能用于表示硬件触发器的状态。
DECLARE_bool(hardware_trigger);

struct StampedTransform {
  double timestamp = 0.0;
  Eigen::Translation3d translation;
  Eigen::Quaterniond rotation;
//EIGEN_MAKE_ALIGNED_OPERATOR_NEW 宏使得在使用 Eigen 类型创建对象时，
//可以自动为这些对象定义对齐的 new 操作符，以提高性能并确保内存对齐。
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class TransformCache {
  public:
    TransformCache() = default;
    ~TransformCache() = default;

    void AddTransform(const StampedTransform& transform);
    bool QueryTransform(double timestamp, StampedTransform* transform, double max_duration = 0.0);

    inline void SetCacheDuration(double duration) { cache_duration_ = duration; }

  protected:
    std::deque<StampedTransform> transforms_;
    double cache_duration_ = 1.0;
};

class TransformWrapper {
  public:
    TransformWrapper() {}
    ~TransformWrapper() = default;

    void Init(const std::string& sensor2novatel_tf2_child_frame_id);
    void Init(const std::string& sensor2novatel_tf2_frame_id,
              const std::string& sensor2novatel_tf2_child_frame_id,
              const std::string& novatel2world_tf2_frame_id,
              const std::string& novatel2world_tf2_child_frame_id_);

    //Attention: must initialize TransformWrapper first
    bool GetSensor2worldTrans(double timestamp,
                              Eigen::Affined3d* sensor2world_trans,
                              Eigen::Affined3d* novatel2world_trans = nullptr);

    bool GetExtrinsics(Eigen::Affined3d* trans);

    //Attention: can be called without initialization
    bool GetTrans(double timestamp, Eigen::Affined3d* trans,
                  const std::string& frame_id, const std::string& child_frame_id);

    bool GetExtrinsicsBySensorId(const std::string& from_sensor_id,
                                 const std::string& to_sensor_id,
                                 Eigen::Affine* trans);
    
    protected:
      bool QueryTrans(double timestamp, StampedTransform* trans,
                      const std::string& frame_id,
                      const std::string& child_frame_id);

  private:
    bool inited_ = false;
    
    Buffer* tf2_buffer_ = Buffer::Instance();
    std::string sensor2novatel_tf2_frame_id_;
    std::string sensor2novatel_tf2_child_frame_id_;
    std::string novatel2world_tf2_frame_id_;
    std::string novatel2world_tf2_child_frame_id_;

    std::unique_ptr<Eigen::Affine3d> snesor2novatel_extrinsics_;

    TransformCache transform_cache;
};

}
}
}