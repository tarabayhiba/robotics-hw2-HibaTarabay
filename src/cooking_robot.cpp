#include "cooking_robot.hpp"
#include <stdexcept>

CookingRobot::CookingRobot(const std::string& id, const std::string& name,
                int battery, int temperature)
    : Robot(id, name, battery), temperature_(temperature) {}

void CookingRobot::work() {
    if (battery_ == 0) {
        throw std::runtime_error("CookingRobot " + id_ + " has no battery left");
    }

    battery_ = battery_ - 15 < 0 ? 0 : battery_ - 15;
    status_ = "working";
}

std::string CookingRobot::type() const {
    return "CookingRobot";
}
