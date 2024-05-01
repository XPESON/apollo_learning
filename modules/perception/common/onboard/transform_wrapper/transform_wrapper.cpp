#include "modules/perception/common/onboard/transform_wrapper/transform_wrapper.h"

#include <limits>

#include "cyber/common/log.h"
#include "modules/common/util/string_util.h"
#include "modules/perception/common/algorithm/sensor_manager/sensor_manager.h"

namespace apollo {
namespace perception {
namespace onboard {

//DEFINE_string 宏，它用于定义一个字符串类型的参数。
//参数名为 obs_sensor2novatel_tf2_frame_id，初始值为 "novatel"，描述信息为 "sensor to novatel frame id"。
//这些宏定义的参数可以通过命令行传递参数值，或者在程序内部进行设置，以控制程序的行为。这种方式可以很方便地修改程序的配置参数，而无需修改源代码。
DEFINE_string(obs_sensor2novatel_tf2_frame_id, "novatel",
              "sensor to novatel frame id");
DEFINE_string(obs_novatel2world_tf2_frame_id, "world",
              "novatel to world frame id");
DEFINE_string(obs_novatel2world_tf2_child_frame_id, "novatel",
              "novatel to world child frame id");
DEFINE_double(obs_tf2_buff_size, 0.01, "query Cyber TF buffer size in second");    
DEFINE_double(obs_transform_cache_size, 1.0, "transform cache size in second");
DEFINE_double(obs_max_local_pose_extrapolation_latency, 0.15,
              "max local pose extrapolation period in second");
DEFINE_bool(obs_enable_local_pose_extrapolation, true,
            "use local pose extrapolation");
DEFINE_bool(hardware_trigger, true, "camera trigger method");
}
}
}