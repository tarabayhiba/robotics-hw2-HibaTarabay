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

}  // namespace

int main() {
    return 0;
}
