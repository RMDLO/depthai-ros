#include "depthai_ros_driver/param_handlers/tof_param_handler.hpp"

#include "depthai-shared/common/CameraBoardSocket.hpp"
#include "depthai/pipeline/datatype/ToFConfig.hpp"
#include "depthai/pipeline/node/Camera.hpp"
#include "depthai/pipeline/node/ToF.hpp"
#include "depthai_bridge/ImuConverter.hpp"
#include "depthai_ros_driver/utils.hpp"
#include "rclcpp/logger.hpp"
#include "rclcpp/node.hpp"

namespace depthai_ros_driver {
namespace param_handlers {
ToFParamHandler::ToFParamHandler(rclcpp::Node* node, const std::string& name) : BaseParamHandler(node, name) {
    fModMap = {{"F_MOD_ALL", dai::ToFConfig::DepthParams::TypeFMod::F_MOD_ALL},
               {"F_MOD_MIN", dai::ToFConfig::DepthParams::TypeFMod::F_MOD_MIN},
               {"F_MOD_MAX", dai::ToFConfig::DepthParams::TypeFMod::F_MOD_MAX}};
}
ToFParamHandler::~ToFParamHandler() = default;
void ToFParamHandler::declareParams(std::shared_ptr<dai::node::Camera> cam, std::shared_ptr<dai::node::ToF> tof) {
    declareAndLogParam<bool>("i_publish_topic", true);
    int socket = declareAndLogParam<int>("i_board_socket_id", 0);
    cam->setBoardSocket(static_cast<dai::CameraBoardSocket>(socket));
    declareAndLogParam<int>("i_width", 640);
    declareAndLogParam<int>("i_height", 480);
    auto tofConf = tof->initialConfig.get();
    tofConf.depthParams.avgPhaseShuffle = declareAndLogParam<bool>("i_avg_phase_shuffle", false);
    tofConf.depthParams.minimumAmplitude = declareAndLogParam<double>("i_minimum_amplitude", 3.0);
    tofConf.depthParams.freqModUsed = utils::getValFromMap(declareAndLogParam<std::string>("i_freq_mod_used", "F_MOD_MAX"), fModMap);
    tof->initialConfig.set(tofConf);
}

dai::CameraControl ToFParamHandler::setRuntimeParams(const std::vector<rclcpp::Parameter>& /*params*/) {
    dai::CameraControl ctrl;
    return ctrl;
}
}  // namespace param_handlers
}  // namespace depthai_ros_driver