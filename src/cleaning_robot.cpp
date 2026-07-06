#include "cleaning_robot.hpp"

CleaningRobot::CleaningRobot(const std::string& id, const std::string& name,
                int battery, double speed, int dirt_capacity)
    : MobileRobot(id, name, battery, speed),
      dirt_capacity_(dirt_capacity < 0 ? 0 : dirt_capacity) {}

void CleaningRobot::work() {
    MobileRobot::work();  
    if (dirt_capacity_ > 0) {
        --dirt_capacity_;
    }
}

std::string CleaningRobot::type() const {
    return "CleaningRobot";
}

std::string CleaningRobot::details() const {
    return MobileRobot::details() + " | Dirt Capacity: " + std::to_string(dirt_capacity_);
}