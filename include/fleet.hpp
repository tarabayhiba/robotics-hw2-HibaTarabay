#pragma once
#include "robot.hpp"
#include "task.hpp"
#include <memory>
#include <string>
#include <ostream>
#include <unordered_map>
#include <queue>

class Fleet {
public:
    void add(std::shared_ptr<Robot> robot);
    void remove(const std::string& id);
    std::shared_ptr<Robot> find(const std::string& id) const;

    // Stores the task in the priority queue and marks the robot as busy.
    void assign_task(const std::string& robot_id, const Task& t);
    void show_tasks()  const;  // prints the task queue in priority order

    void work_all()    const;  // calls work() on every robot
    void charge_all()  const;  // calls charge() on every robot

    std::size_t size()  const;
    bool        empty() const;

    Fleet& operator+=(std::shared_ptr<Robot> robot);
    Fleet& operator-=(const std::string& id);
    friend std::ostream& operator<<(std::ostream& os, const Fleet& f);

private:
    // since robots need fast lookup by id
    // unordered_map (hash table implementation) is the only option offering O(1) 
    //lookup/insert/earase by id without paying for order it doesn't need

    std::unordered_map<std::string, std::shared_ptr<Robot>> robots_;
    // since tasks need to be ordered by priority--> a priority queue is the only option
    // priority_queue is a binary heap that keeps the highest priority task at the top
    // Adding or removing a task takes O(log n), while accessing the top task takes O(1
    std::priority_queue<Task> tasks_;
};