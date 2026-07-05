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

}  // namespace

int main() {
    Fleet fleet;

    while (true) {
        print_menu();
        int choice = read_int("Choice: ");

        if (choice == 0) {
            break;
        }

        std::cout << "Option not implemented yet.\n";
    }

    return 0;
}
