#include "Eigen/Core"

#include "modules/perception/common/base/object_supplement.h"
#include "modules/perception/common/base/object_types.h"
#include "modules/perception/common/base/point_cloud.h"
#include "modules/perception/common/base/vehicle_struct.h"
// #include "modules/common_msgs/prediction_msgs/feature.pb.h"

namespace apollo {
namespace perception {
namespace base {
  struct Object {
    Object();
    std::string ToString() const;
    void Reset();

    //@brief object id per frame, require
    int id = -1;

    //@brief convex hull of the object, required
    /*"convex hull of the object, required"（对象的凸包，必需）通常表示在特定上下文中对对象的凸包进行计算是必需的。
    在计算机图形学、计算几何学和计算机视觉中，凸包（convex hull）是指包含对象所有点的最小凸多边形或凸多面体。凸包的计算可以帮助识别对象的整体形状，
    并在很多应用中起着重要的作用。
    "required"（必需）则表示在某些任务或场景中，计算对象的凸包是必需的。这可能是因为凸包提供了关于对象形状、轮廓或边界的重要信息，
    或者因为后续的算法或处理步骤需要凸包作为输入。
    例如，在目标检测和识别中，计算目标的凸包可以帮助确定目标的整体形状，并且可以用于提取特征或进行形状匹配。在机器人导航中，计算环境中障碍物的凸包可以帮助规划避障路径。
    在三维重建中，计算点云数据的凸包可以帮助识别物体的轮廓和表面。因此，"convex hull of the object, required" 表示在特定上下文中，计算对象的凸包是必需的，
    可能是为了支持后续的处理或分析步骤，或者提供关于对象形状的重要信息。*/
    PointCloud<PointD> polygon;

    // oriented boundingbox information
    //@brief main direction of the object, required
    Eigen::Vector3f directioon = Eigen::Vector3f(1, 0, 0);

    /*
    the yaw angle, theta = 0.0 <=> direction(1, 0, 0),
    currently roll and pitch are not considered,
    make sure direction and theta are consistent, required
    */
   float theta = 0.0f;
   //@brief theta variance, required
   float theta_variance = 0.0f;
   //@brief center of the boundingbox (cx, cy, cz), required
   Eigen::Vector3d center = Eigen::Vector3d(0, 0, 0);
   //@brief convariance matrix of the center uncertainty, required
   Eigen::Matrix3f center_uncertainty;
   /*@brief size = [length, width, height] of boundingbox
     length is the size of the main direction, required*/
    Eigen::Vector3f size = Eigen::Vector3f(0, 0, 0);
    // @brief size variance, required
    Eigen::Vector3f size_variance = Eigen::Vector3f(0, 0, 0);
    // @brief anchor point, required
    /*"anchor point"（锚点）通常指的是在空间中固定的、用作参考的点或位置。它可以是任意对象的一个特定点，用于表示或定位该对象的位置、方向或特征。*/
    Eigen::Vector3d anchor_point = Eigen::Vector3d(0, 0, 0);

    //@brief object type, required
    ObjectType type = ObjectType::UNKNOWN;
      // @brief probability for each sub-type, optional
    std::vector<float> sub_type_probs;

    // @brief existence confidence, required
    float confidence = 1.0f;

    //tracking information
    //@brief track id, required
    int track_id = -1;
    //@brief velocity of the object, required
    Eigen::Vector3f velocity = Eigen::Vector3f(0, 0, 0);
    // @brief covariance matrix of the velocity uncertainty, required
    Eigen::Matrix3f velocity_uncertainty;




  }
}
}
}