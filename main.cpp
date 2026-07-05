#include "cleaning_robot.hpp"
#include "cooking_robot.hpp"
#include "fleet.hpp"
#include "mobile_robot.hpp"
#include "robot.hpp"
#include "task.hpp"
#include <iostream>
#include <memory> // std::make_shared, std::dynamic_pointer_cast
#include <limits> //  std::numeric_limits, for the cin.ignore() recovery
#include <string>
#include <stdexcept>

namespace {

void clear_bad_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int read_int(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value) {
            return value;
        }
        clear_bad_input();
        std::cout << "Invalid number, try again.\n";
    }
}

double read_double(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        double value;
        if (std::cin >> value) {
            return value;
        }
        clear_bad_input();
        std::cout << "Invalid number, try again.\n";
    }
}

std::string read_nonempty_string(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string value;
        std::cin >> value;
        if (!value.empty()) {
            return value;
        }
        std::cout << "Input cannot be empty, try again.\n";
    }
}

void print_menu() {
    std::cout << "\n=== Robot Fleet Manager ===\n"
               << "1. Add robot\n"
               << "2. Remove robot\n"
               << "3. Show all robots\n"
               << "4. Work (single robot)\n"
               << "5. Work all\n"
               << "6. Charge all\n"
               << "7. Assign task to robot\n"
               << "8. Show task queue\n"
               << "9. Start timed work on a robot\n"
               << "0. Exit\n";
}

void add_robot(Fleet& fleet) {
    int type = read_int("Robot type (1=Mobile, 2=Cleaning, 3=Cooking): ");
    if (type < 1 || type > 3) {
        std::cout << "Unknown robot type.\n";
        return;
    }

    std::string id      = read_nonempty_string("id: ");
    std::string name    = read_nonempty_string("name: ");
    int         battery = read_int("battery (0-100): ");

    if (type == 1) {
        double speed = read_double("speed: ");
        fleet.add(std::make_shared<MobileRobot>(id, name, battery, speed));
    } else if (type == 2) {
        double speed         = read_double("speed: ");
        int    dirt_capacity = read_int("dirt capacity: ");
        fleet.add(std::make_shared<CleaningRobot>(id, name, battery, speed, dirt_capacity));
    } else {
        int temperature = read_int("cooking temperature (C): ");
        fleet.add(std::make_shared<CookingRobot>(id, name, battery, temperature));
    }
    std::cout << "Robot added.\n";
}

void remove_robot(Fleet& fleet) {
    std::string id = read_nonempty_string("id to remove: ");
    try {
        fleet.find(id);           // throws if the id doesn't exist
        fleet.remove(id);
        std::cout << "Robot removed.\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void work_single(Fleet& fleet) {
    std::string id = read_nonempty_string("id to work: ");
    try {
        auto robot = fleet.find(id);
        robot->work();
        std::cout << *robot << "\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void assign_task(Fleet& fleet) {
    std::string name = read_nonempty_string("task name: ");
    int priority;
    while (true) {
        priority = read_int("priority (1-5): ");
        if (priority >= 1 && priority <= 5) {
            break;
        }
        std::cout << "Priority must be between 1 and 5.\n";
    }
    std::string id = read_nonempty_string("robot id: ");

    try {
        fleet.assign_task(id, Task{name, priority, id});
        std::cout << "Task assigned.\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void start_timed_work(Fleet& fleet) {
    std::string id = read_nonempty_string("id to start timed work on: ");
    try {
        auto robot  = fleet.find(id);
        auto mobile = std::dynamic_pointer_cast<MobileRobot>(robot);
        if (!mobile) {
            std::cout << "Robot " << id << " cannot move, it has no timed work.\n";
            return;
        }

        int seconds = read_int("duration in seconds: ");
        if (seconds <= 0) {
            std::cout << "Duration must be a positive number of seconds.\n";
            return;
        }

        mobile->start_work(seconds);
        std::cout << "Timed work started.\n";
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

}

int main() {
    Fleet fleet;

    while (true) {
        print_menu();
        int choice = read_int("Choice: ");

        if (choice == 0) {
            break;
        }

        switch (choice) {
            case 1: add_robot(fleet);        break;
            case 2: remove_robot(fleet);     break;
            case 3: std::cout << fleet;      break;
            case 4: work_single(fleet);      break;
            case 5: fleet.work_all();        break;
            case 6: fleet.charge_all();      break;
            case 7: assign_task(fleet);      break;
            case 8: fleet.show_tasks();      break;
            case 9: start_timed_work(fleet); break;
            default: std::cout << "Unknown option.\n"; break;
        }
    }

    return 0;
}
