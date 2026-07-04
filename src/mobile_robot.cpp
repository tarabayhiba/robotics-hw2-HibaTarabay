#include "mobile_robot.hpp"

MobileRobot::MobileRobot(const std::string& id, const std::string& name,
                int battery, double speed)
    : Robot(id, name, battery), speed_(speed) {}

void MobileRobot::work() {

    status_="working";

}  // moves, uses 20% battery

std::string MobileRobot::type()  const {
    return "MobileRobot";
}

void MobileRobot::start_work(int seconds){


}