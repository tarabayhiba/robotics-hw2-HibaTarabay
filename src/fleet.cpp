#include "fleet.hpp"
#include <stdexcept>
#include <algorithm>
#include <iostream>

void Fleet::add(std::shared_ptr<Robot> robot) {
    robots_[robot->id()] = robot;
}

void Fleet::remove(const std::string& id) {
    robots_.erase(id);
}

std::shared_ptr<Robot> Fleet::find(const std::string& id) const {
    auto it = robots_.find(id);
    if (it == robots_.end()) {
        throw std::runtime_error("Robot with id " + id + " not found");
    }
    return it->second;
}

void Fleet::assign_task(const std::string& robot_id, const Task& t) {
    auto robot = find(robot_id);  // throws before any state changes if id is bad

    Task assigned = t;
    assigned.assigned_to = robot_id;
    tasks_.push(assigned);

    robot->work();  // marks robot as busy
}

void Fleet::show_tasks() const {
    std::priority_queue<Task> copy = tasks_;  // priority_queue has no iterator --> print from a copy
    while (!copy.empty()) {
        std::cout << copy.top() << std::endl;
        copy.pop();
    }
}

void Fleet::work_all() const {
    auto low_battery = std::find_if(robots_.begin(), robots_.end(),
        [](const auto& pair) { return pair.second->battery() < 20; });
    if (low_battery != robots_.end()) {
        std::cout << "Warning: " << low_battery->second->name()
                   << " is below 20% battery\n";
    }

    for (const auto& [id, robot] : robots_) {
        try {
            robot->work();
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

void Fleet::charge_all() const {
    for (const auto& [id, robot] : robots_) {
        robot->charge();
    }
}

std::size_t Fleet::size() const {
    return robots_.size();
}

bool Fleet::empty() const {
    return robots_.empty();
}

Fleet& Fleet::operator+=(std::shared_ptr<Robot> robot) {
    add(robot);
    return *this;
}

Fleet& Fleet::operator-=(const std::string& id) {
    remove(id);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Fleet& f) {
    os << "Fleet (" << f.robots_.size() << " robots):\n";
    for (const auto& [id, robot] : f.robots_) {
        os << *robot << "\n";
    }
    return os;
}
