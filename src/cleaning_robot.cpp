#include "cleaning_robot.hpp"

CleaningRobot::CleaningRobot(const std::string& id, const std::string& name,
                int battery, double speed, int dirt_capacity)
    : MobileRobot(id, name, battery, speed), dirt_capacity_(dirt_capacity) {}

void CleaningRobot::work() {
    MobileRobot::work();  // moves and drains battery like a MobileRobot
    if (dirt_capacity_ > 0) {
        --dirt_capacity_;
    }
}

std::string CleaningRobot::type() const {
    return "CleaningRobot";
}