#pragma once
#include "robot.hpp"

class CookingRobot : public Robot {
public:
    CookingRobot(const std::string& id, const std::string& name,
                 int battery, int temperature);

    void        work()  override;  // cooks uses 15% battery
    std::string type()  const override;

protected:
    int temperature_;  // in Celsius
};
