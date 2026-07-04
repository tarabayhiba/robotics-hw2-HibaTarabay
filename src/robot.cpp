#include "robot.hpp"
#include <iostream>

Robot::Robot(const std::string& id, const std::string& name, int battery)
    : id_(id), name_(name),
      battery_(battery < 0 ? 0 : (battery > 100 ? 100 : battery)),
      status_("idle") {}


std::string Robot::id()      const{
    return id_;
}
std::string Robot::name()    const{
    return name_;
}
int         Robot::battery() const{
    return battery_;
}
std::string Robot::status()  const{
    return status_;
}

void  Robot::charge(){
    battery_=100;
}

bool Robot::operator==(const Robot& other) const {
    return id_ == other.id_;
}

std::string Robot::operator+(const Robot& other) const {
    return name_ + " + " + other.name_;
}

/*  from hw session 2 file:
"`friend` gives a specific function access to the private members of 
a class without being a member of it.
`operator<<` must be a free function because the left side of `<<`
is `std::ostream`, not your class"
LHS of << is std::ostream not Robot--> can't be a method on Robot
--> it must be a free function
--> needs friend to reach the protected member */
std::ostream& operator<<(std::ostream& os, const Robot& r) {
    os << r.id_ << " " << r.name_ << " " << r.type() << " "
       << r.battery_ << " " << r.status_;
    return os;
}

// `work()` & `type()` pure virtual nothing to implement here 