#pragma onece

#include <string>

namespace apollo {
namespace perception {
namespace base {
/**
 * @brief Sensor types are set in the order of lidar, radar, camera, ultrasonic
 *        Please make sure SensorType has same id with SensorMeta,
 *        which defined in the proto of sensor_manager
 */
enum class SensorType {
  UNKNOWN_SENSOR_TYPE = -1,
  VELODYNE_128 = 0,
  VELODYNE_64 = 1,
  VELODYNE_32 = 2,
  VELODYNE_16 = 3,
  LDLIDAR_4 = 4,
  LDLIDAR_1 = 5,
  SHORT_RANGE_RADAR = 6,
  LONG_RANGE_RADAR = 7,
  MONOCULAR_CAMERA = 8,
  STEREO_CAMERA = 9,
  ULTRASONIC = 10,
  SENSOR_TYPE_NUM
};

enum class SensorOrientation {
  FRONT = 0,
  LEFT_FORWARD = 1,
  LEFT = 2,
  LEFT_BACKWARD = 3,
  REAR = 4,
  RIGHT_BACKWARD = 5,
  RIGHT = 6,
  RIGHT_FORWARD = 7,
  PANORAMIC = 8
};

struct SensorInfo {
  std::string name = "UNKNOWN_SENSOR";
  SensorType type = SensorType::UNKNOWN_SENSOR_TYPE;
  SensorOrientation orientation = SensorOrientation::FRONT;
  std::string frame_id = "UNKNOWN_FRAME_ID";
  bool is_main_sensor = false;
};


}
}
}