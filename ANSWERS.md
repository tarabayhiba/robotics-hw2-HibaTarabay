# Part B — Answers

**1. Why `shared_ptr` in Fleet and not `unique_ptr`?**
`Fleet::find()` returns a `std::shared_ptr<Robot>` by value, and `add()`/`operator+=` take one by value too. That means ownership needs to be shared, the `robots_` map holds one owning reference, & for example `assign_task`, `main.cpp`'s `mobile->start_work` gets its own owning copy. The robot stays alive even if it's later removed from the fleet while that caller still holds it. `unique_ptr` only allows a single owner, so `find()` couldn't return a copy at all, it would have to return a pointer/reference instead (no shared ownership, and it dangles after `remove()`), or hand ownership out of the map entirely (which would empty the fleet's slot). Switching to `unique_ptr` wouldn't even compile since both `find()`'s return by copy and `add(shared_ptr<Robot>)`'s by value copy require a copyable smart pointer

**2. What container did you use for `robots_`? Why?**
The fleet never needs the robots in a specific order, so for an ordered structure for example `std::map`, O(log n) would be wasted cost. `std::unordered_map<std::string, std::shared_ptr<Robot>>` (`fleet.hpp:35`). Lookup/insert/erase by robot id is O(1) average since it's a hash table, which is what `find()`, `add()`, and `remove()` need

**3. What container did you use for the task queue?**
`std::priority_queue<Task>` (`fleet.hpp:39`), built on a binary heap `Task::operator<` orders by `priority` (`task.cpp`), so the heap keeps the highest priority task at the top `push`/`pop` are O(log n), and `top()` (highest priority) is O(1) which is exactly "highest priority first" with no need to scan or sort the whole queue

**4. Why is `operator<<` `friend` and not a regular member?**
`std::cout << robot` --> `operator<<(std::cout, robot)` where the left operand is `std::ostream`, not `Robot`, so it can't be a member of `Robot` (a member call would need `robot << std::cout`). `operator<<` prints `r.id_`, `r.name_`, `r.battery_`, `r.status_`, which are `protected`, so free function couldn't touch them. `friend` allows that one free function access to Robot's non public members without making it a member itself

**5. `std::find_if` with a lambda vs. a hand-written loop**
In `Fleet::work_all()`, find_if` with `[](const auto& pair){ return pair.second->battery() < 20; } directly finds the first element matching this predicate & separating from the manual management of iterating. A hand-written `for` loop with a manual `break`/flag mixes the traversal logic with the condition --> more error-prone (off-by-one, forgetting to break), and reads less immediately than an STL algorithm

**6. Why does `work()` throw instead of returning false / printing an error?**
 Zero battery is a real failure, not a normal outcome the caller should check every time.`bool` return is easy to ignore, silently leaving the fleet in a wrong state, and printing directly from `work()` hard codes I/O into a low-level method. An exception forces callers to explicitly decide how to handle the failure (or propagate it), and callers that do care can catch it exactly where it matters  `Fleet::work_all()` catches `std::runtime_error` per robot so one dead robot doesn't stop the rest (fleet.cpp), while `start_work`'s thread catches it to stop cleanly (`mobile_robot.cpp).

**7. Is Fleet's state consistent if `assign_task` throws?**
Yes, `assign_task` calls `find(robot_id)` first (`fleet.cpp`), and `find` is what throws (`fleet.cpp`) when the id isn't in `robots_`. That throw happens before `tasks_.push(assigned)` or `robot->work()` run, so no task is queued and no robot state changes. The exception aborts the function before any mutation, leaving `robots_` and `tasks_` exactly as they were

**8. `start_work` background thread — what did you do with `stop_`/`worker_`, and why?**
`worker_` is the `std::thread` running the loop
`stop_` is `std::atomic<bool>` used as a cooperative cancellation flag, since the thread can't be killed forcibly
`start_work` first joins any previous `worker_` if still joinable (so a robot can't have two workers running at once), resets `stop_ = false`, then launches a new thread that loops up to `seconds` times, checking `stop_` each iteration and breaking if it's true (`mobile_robot.cpp`). 
The destructor sets `stop_ = true` and joins `worker_`, so a `MobileRobot` being destroyed while its worker thread is running doesn't leave a dangling detached thread or crash. It signals the thread to stop and waits for it to actually finish first. `stop_` is `atomic` because it's read/written from two different threads (the worker thread and whichever thread calls the destructor/`start_work`), so a plain `bool` would be a data race.

**9. Diamond inheritance**

```cpp
class Robot { public: std::string name_ = "base"; };
class MobileRobot      : public Robot {};
class CookingRobot     : public Robot {};
class MaintenanceRobot : public MobileRobot, public CookingRobot {};

int main() {
    MaintenanceRobot m;
    std::cout << m.name_;   // which name_?
}
```

This doesn't compile because `m.name_` is ambiguous: `MaintenanceRobot` inherits from both `MobileRobot` and `CookingRobot`, and each of those has its own independent `Robot` base subobject, so `MaintenanceRobot` contains 2 different copies of `Robot` one because of `MobileRobot::Robot::name_`, & other one through `CookingRobot::Robot::name_`. `m.name_` doesn't tell the compiler which one, so it's a compile error

C++ solves this with virtual inheritance, `class MobileRobot : public virtual Robot {}` and `class CookingRobot : public virtual Robot {}`. `virtual` means "share the base subobject" both `MobileRobot` and `CookingRobot` now point to the same single `Robot` subobject instead of each carrying their own, so `MaintenanceRobot` contains exactly one `Robot`/`name_`, and `m.name_` 

```cpp
class Robot { public: std::string name_ = "base"; };
class MobileRobot      : public virtual Robot {};
class CookingRobot     : public virtual Robot {};
class MaintenanceRobot : public MobileRobot, public CookingRobot {};

int main() {
    MaintenanceRobot m;
    std::cout << m.name_;   
}
```