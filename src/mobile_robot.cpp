#include "mobile_robot.hpp"
#include <stdexcept>
#include <chrono>
#include <iostream>

MobileRobot::MobileRobot(const std::string& id, const std::string& name,
                int battery, double speed)
    : Robot(id, name, battery), speed_(speed < 0 ? 0 : (speed > 100 ? 100 : speed)) {}

     

MobileRobot::~MobileRobot() {
    stop_ = true;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void MobileRobot::work() {
    if (battery_ == 0) {
        throw std::runtime_error("MobileRobot " + id_ + " has no battery left");
    }

    battery_ = battery_ - 20 < 0 ? 0 : battery_ - 20;
    status_="working";

}  // moves, uses 20% battery

std::string MobileRobot::type()  const {
    return "MobileRobot";
}

std::string MobileRobot::details() const {
    return "Speed: " + std::to_string(speed_);
}

void MobileRobot::start_work(int seconds){
    if (worker_.joinable()) {
        worker_.join();
    }
    stop_ = false;
    worker_ = std::thread([this, seconds]{
        for (int i = 0; i < seconds; ++i) {
            if (stop_) {
                break;
            }
            try {
                work();
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
                break;
            }
            std::cout << *this << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}
