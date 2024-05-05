#ifndef CYBER_TRANSPORT_COMMON_ENDPOINT_H_
#define CYBER_TRANSPORT_COMMON_ENDPOINT_H_

#include <memory>
#include <string>

#include "cyber/proto/role_attributes.pb.h"

#include "cyber/transport/common/identity.h"

namespace apollo {
namespace cyber {
namespace transport {

class Endpoint;
using EndpointPtr = std::shared_ptr<Endpoint>;
using proto::RoleAttributes;

class Endpoint {
 public:
  explicit Endpoint(const RoleAttributes& attr);
  virtual ~Endpoint();

  const Identity& id() const { return id_; }
  const RoleAttributes& attributes() const { return attr_; }

 protected:
  bool enabled_;
  Identity id_;
  RoleAttributes attr_; 
};

}
}
}