#include <iostream>
#include <string>

#include "Errors.hpp"
#include "ServiceCenter.hpp"

int main() {
  ServiceCenter sc;
  while (true) {
    std::string command;
    std::cin >> command;
    if (command == "help") {
      std::cout
          << "Available commands:\n"
          << "add <type> <id> <name> [energy] [score] - add item to fleet\n"
          << "  types: scooter <id> <name> <energy> <score>\n"
          << "         bike <id> <name> <score>\n"
          << "         ebike <id> <name> <energy> <score>\n"
          << "         speedscooter <id> <name> <energy>\n"
          << "         helmet <id> <name>\n"
          << "         dock <id> <name> <energy>\n"
          << "energy - total daily energy consumption\n"
          << "novice - list of items suitable for novices (score >= 6)\n"
          << "list - list all items sorted by inventory id\n"
          << "focus <id> - set focus on item\n"
          << "show_focus - show focused item or removed/expired\n"
          << "remove <id> - remove item from fleet\n"
          << "last_added - 5 last added items\n"
          << "last_deleted - 5 last deleted items\n"
          << "quit - exit";
    } else if (command == "add") {
      std::string args;
      std::getline(std::cin, args);
      try {
        std::cout << sc.add(args) << '\n';
      } catch (const InspectionFailed& e) {
        std::cout << "inspection rejected:" << e.what() << '\n';
      } catch (const InvalidArgument& e) {
        std::cout << "error:" << e.what() << '\n';
      }
    } else if (command == "energy") {
      std::cout << sc.energy() << '\n';
    } else if (command == "novice") {
      std::cout << sc.novice() << '\n';
    } else if (command == "list") {
      std::cout << sc.list() << '\n';
    } else if (command == "focus") {
      std::string id;
      std::cin >> id;
      try {
        std::cout << sc.focus(id) << '\n';
      } catch (const InvalidArgument& e) {
        std::cout << "error:" << e.what() << '\n';
      }
    } else if (command == "show_focus") {
      std::cout << sc.show_focus() << '\n';
    } else if (command == "remove") {
      std::string id;
      std::cin >> id;
      try {
        std::cout << sc.remove(id) << '\n';
      } catch (const InvalidArgument& e) {
        std::cout << "error:" << e.what() << '\n';
      }
    } else if (command == "last_added") {
      std::cout << sc.last_added();
    } else if (command == "last_deleted") {
      std::cout << sc.last_deleted();
    } else if (command == "quit") {
      return 0;
    } else {
      std::cout << "unknown command\n";
    }
  }
}