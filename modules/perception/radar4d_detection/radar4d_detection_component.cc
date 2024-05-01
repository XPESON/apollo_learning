#include "modules/perception/radar4d_detection/radar4d_detection_component.h"

#include "cyber/time/clock.h"
#include "modules/common/util/perf_util.h"
#include "modules/perception/common/algorithm/sensor_manager/sensor_manager.h"
#include "modules/perception/common/onboard/common_flags/common_flags.h"

using Clock = apollo::cyber::Clock;

namespace apollo {
namespace perception {
namespace radar4d {

std::atomic<uint32_t> Radar4dDectionComponent::seq_num_{0};

bool Radar4dDetectionComponent::Init() {
  Radar4dDectionConfig comp_config;
  if(!GetProtoConfig(&comp_config)) {//非重点，带补充
    return false;
  }
  AINFO <<"Radar4d Detection Component Config: " << comp_config.DebugString();
    tf_child_frame_id_ = comp_config.tf_child_frame_id();
  radar_forward_distance_ = comp_config.radar_forward_distance();
  odometry_channel_name_ = comp_config.odometry_channel_name();

  // Load sensor info
  if (!algorithm::SensorManager::Instance()->GetSensorInfo(
          comp_config.radar_name(), &radar_info_)) {
    AERROR << "Failed to get sensor info, sensor name: "
           << comp_config.radar_name();
    return false;
  }

  // Create writer
  writer_ = node_->CreateWriter<onboard::SensorFrameMessage>(
      comp_config.output_channel_name());

  // Init algorithm plugin
  ACHECK(InitAlgorithmPlugin(comp_config))
      << "Failed to init algorithm plugin.";

  // Init localization config
  radar2world_trans_.Init(tf_child_frame_id_);
  radar2novatel_trans_.Init(tf_child_frame_id_);
  localization_subscriber_.Init(
      odometry_channel_name_,
      odometry_channel_name_ + '_' + comp_config.radar_name());
  return true;
}
bool Radar4dDetectionComponent::Proc(const std::shared_ptr<drivers::OculiiPointCloud>& message) {
  AINFO << "Enter radar preprocess, message timestamp: "
        << message->header().timestamp_sec() << " current timestamp"
        << Clock::NowInstance();
  auto out_message = std::make_shared<onboard::SensorFrameMessage>()
}
}
}
}