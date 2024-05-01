#pragma once

#include <memory>
#include <string>
#include <vector>

#include "modules/perception/common/base/sensor_meta.h"
namespace apollo {
namespace perception {
namespace radar4d {
 
using apollo::drivers::OculiiPointCloud;
using apollo::localization::LoacalizationEstimate;

class Radar4dDectionComponent : public cyber::Component<drivers::OculiiPointCloud> {
  public:
    Radar4dDetectionComponent()
        : tf_child_frame_id_(""),
          radar_forword_distance_(200.0),
          odometry_channel_name_(""),
          hdmap_input_(nullptr),
          radar_preprocessor_(nullptr),
          radar_perception_(nullptr) {}
    ~Radar4dDectionComponent() = default;

    bool Init() override;
    bool Proc(const std::shared_ptr<drivers::OculiipointCloud>& message) override;

  private:
    bool InitAlgotithmPlugin(const Radar4dDectionConfig& config);
    bool InternalProc(const std::shared_ptr<const driver::OculiiPointCloud>& in_message,
                      std::shared_ptr<onboard::SensorFrameMessage> out_message);
    bool GetCarLocalizationSpeed(double timestamp,
                                 Eigen::Vetor3f* car_linear_speed,
                                 Eigen::Vector3f* car_angular_speed);
    Radar4dDetectionComponent(const Radar4dDetectionComponent&) = delete;
    Radar4dDetectionComponent& operator=(const Radar4dDectionComponent&) = delete;

  private:
    static std::atomic<uint32_t> seq_num;

    base::SensorInfo radar_info_;
    std::string tf_child_frame_id_;
    double radar_forward_distance;
    std::string odometry_channel_name_;

    onboard::TransformWraper radar2world_trans_;
    onboard::TransformWraper radar2novatel_trans_
    map::HDMapInput* hdmap_input_;
    std::shared_ptr<BasePreprocessor> radar_preprocessor_;
    std::shared_ptr<BaseRadarObstaclePerception> radar_perception_;
    onboard::MsgBuffer<LocalizationEstimate> localization_subscriber_;
    std::shared_ptr<apollo::cyber::Write<onboard::Sensor::SensorFrameMessage>> writer_;
};
CYBER_REGISTER_COMPONENT(Radar4DDetectionComponent);
}
}
}
