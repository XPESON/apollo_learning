#pragma once

#include <memory>
#include <vector>

#include "Eigen/Dense"

#include "modules/perception/common/base/frame_supplement.h"
#include "modules/perception/common/base/object.h"
#include "modules/perception/common/base/object_types.h"
#include "modules/perception/common/base/sensor_meta.h"

namespace apollo {
namespace perception {
namespace base {

struct Frame {
  Frame() { sensor2world_pose.setIdentity(); }

  void Reset() {
    timestamp = 0.0;
    objects.clear();
    sensor2world_pose.setIdentity();
    sensor_info.Reset();
    lidar_frame_supplement.Reset();
    radar_frame_supplement.Reset();
    camera_frame_supplement.Reset();
  }

  //@brief snesor information
  SensorInfo sensor_info;

  double timestamp = 0.0;
  std::vector<std::shared_ptr<Object>> objects;
  Eigen::Affine3d sensor2world_pose;

  //sensor-specific frame supplements
  /*
  "sensor-specific frame supplements"（传感器特定帧补充）通常指的是与特定传感器相关的额外信息或数据，这些信息可以补充或增强传感器产生的数据。

  在感知系统中，不同类型的传感器（如雷达、摄像头、激光雷达等）可能会产生不同类型的数据，这些数据可能需要额外的信息或处理才能更好地理解和利用。
  传感器特定帧补充就是为了满足这种需求而设计的。例如，对于相机传感器，除了图像数据之外，还可以提供其他信息，如相机的内参、外参、畸变参数等。
  这些信息对于图像处理、相机定标和三维重建等任务非常重要。因此，在处理相机数据时，可能需要使用传感器特定的帧补充来获取这些信息。

  类似地，对于雷达传感器，除了原始的点云数据之外，还可以提供雷达的参数、扫描模式、解调信息等。这些信息对于目标检测、跟踪和定位等任务也非常重要。

  因此，传感器特定帧补充是为了提供与特定传感器相关的额外信息或数据，以增强传感器产生的数据的理解和利用能力。
  */
  LidarFrameSupplement lidar_frame_supplement;
  RadarFrameSupplement radar_frame_supplement;
  CameraFrameSupplement camera_frame_supplement;
  UltrasonicFrameSupplement ultrasonic_frame_supplement;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
typedef std::shared_ptr<Frame> FramePtr;
typedef std::shared_ptr<const Frame> FrameConstPtr;
}
}
}