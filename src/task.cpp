#include "task.hpp"

 bool Task::operator<(const Task& other) const{
    return priority < other.priority;
 }

std::ostream& operator<<(std::ostream& os, const Task& t) {
    os << t.name << " " << t.priority << " " << t.assigned_to;
    return os;
}