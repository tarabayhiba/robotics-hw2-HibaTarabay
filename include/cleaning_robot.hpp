#pragma once
#include "mobile_robot.hpp"

class CleaningRobot : public MobileRobot {
public:
    CleaningRobot(const std::string& id, const std::string& name,
                  int battery, double speed, int dirt_capacity);

    void        work()  override;  // vacuums uses 20% battery
    std::string type()  const override;

protected:
    int dirt_capacity_;  // liters of dirt the bin can hold before it needs emptying
};