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
            case 1: add_robot(fleet);    break;
            case 2: remove_robot(fleet); break;
            case 3: std::cout << fleet;  break;
            default: std::cout << "Unknown option.\n"; break;
        }
    }

    return 0;
}
