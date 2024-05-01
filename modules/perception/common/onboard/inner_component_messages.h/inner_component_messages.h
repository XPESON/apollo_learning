#pragma once

#include <string>

#include "cyber/cyber.h"
#include "modules/perception/common/base/frame.h"
#include "modules/perception/common/base/hdmap_struct.h"
#include "modules/perception/common/base/impending_collision_edge.h"
#include "modules/common_msgs/perception_msgs/perception_obstacle.pb.h"

namespace apollo {
namespace perception {
namespace onboard {

enum class ProcessStage {
  LIDAR_PREPROCESS = 0,
  LIDAR_DETECTION = 1,
  LIDAR_RECOGNITION = 2,
  STEREO_CAMERA_DETECTION = 3,
  MONOCULAR_CAMERA_DETECTION = 4,
  LONG_RANGE_RADAR_DETECTION = 5,
  SHORT_RANGE_RADAR_DETECTION = 6,
  ULTRASONIC_DETECTION = 7,
  SENSOR_FUSION = 8,
  UNKNOWN_STAGE = 9,
  PROCESSSTAGE_COUNT = 10
};

class Descriptor {
 public:
  std::string full_name() { return "name"; }
}

class SensorFrameMessage {
 public:
  SensorFrameMessage() { type_name_ = "SensorFrameMessage"; }
  ~SensorFrameMessage() = default;
  std::string GetTypeName() { return type_name_; }
  //这个语句创建了一个动态分配的 SensorFrameMessage 对象，
  //但是不会调用类的构造函数。这意味着对象的内存被分配，但是对象的成员变量（如果有）不会被初始化，也不会执行构造函数中的任何代码。
  //new SensorFrameMessage()会调用默认构造函数
  SensorFrameMessage* New() const { return new SensorFrameMessage; }

  public:
   apollo::common::ErrorCode error_code_ = apollo::common::ErrorCode::OK;

   std::string sensor_id_;
   double timestamp_ = 0.0;
   uint64_t lidar_timestamp_ = 0;
   uint32_t seq_num_ = 0;
   std::string type_name_;
   base::HdmapStructConstPtr hdmap_;

   base::FramePtr frame_;

   ProcessStage process_stage_ = ProcessStage::UNKNOWN_STAGE;
};

}
}
}
