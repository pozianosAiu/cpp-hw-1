#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Errors.hpp"
#include "Inventory.hpp"
#include "Registry.hpp"
#include "Things.hpp"
#include "Transport.hpp"

class ServiceCenter {
 private:
  std::unordered_map<std::string, std::shared_ptr<InventoryItem>> storage_;
  std::weak_ptr<InventoryItem> focus_;
  Registry<std::weak_ptr<InventoryItem>, 5> last_added_;
  Registry<std::unique_ptr<InventoryItem>, 5> last_deleted_;

  static std::vector<std::string> divide(const std::string& s) {
    std::vector<std::string> res{};
    const std::string spaces = " \t\n\r\f\v";

    size_t start = s.find_first_not_of(spaces);
    while (start != std::string::npos) {
      size_t end = s.find_first_of(spaces, start);
      res.push_back(s.substr(start, end - start));
      start = s.find_first_not_of(spaces, end);
    }

    return res;
  }
  double countEnergy() {
    double res{};
    for (auto item : storage_) {
      res += item.second->getEnergy();
    }
    return res;
  }

 public:
  std::string add(const std::string& args) {
    std::vector<std::string> args_divided = divide(args);
    if (args_divided.empty()) {
      throw InvalidArgument("empty arguments");
    }
    if (args_divided[0] == "scooter") {
      if (args_divided.size() != 5)
        throw InvalidArgument("wrong arguments count for scooter");
      const std::string& id = args_divided[1];
      const std::string& name = args_divided[2];
      if (storage_.count(id) > 0) throw InvalidArgument("id already taken");
      double energy;
      int score;
      try {
        energy = std::stod(args_divided[3]);
      } catch (const std::exception&) {
        throw InvalidArgument("energy is not a number");
      }
      try {
        score = std::stoi(args_divided[4]);
      } catch (const std::exception&) {
        throw InvalidArgument("score is not integer");
      }
      std::shared_ptr<Scooter> item =
          std::make_shared<Scooter>(id, name, energy, score);
      storage_[id] = item;
      last_added_.add(item);
      return "added " + id;
    } else if (args_divided[0] == "bike") {
      if (args_divided.size() != 4)
        throw InvalidArgument("wrong arguments count for bike");
      const std::string& id = args_divided[1];
      const std::string& name = args_divided[2];
      if (storage_.count(id) > 0) throw InvalidArgument("id already taken");
      int score;
      try {
        score = std::stoi(args_divided[3]);
      } catch (const std::exception&) {
        throw InvalidArgument("score is not integer");
      }
      std::shared_ptr<Bike> item = std::make_shared<Bike>(id, name, score);
      storage_[id] = item;
      last_added_.add(item);
      return "added " + id;
    } else if (args_divided[0] == "ebike") {
      if (args_divided.size() != 5)
        throw InvalidArgument("wrong arguments count for ebike");
      const std::string& id = args_divided[1];
      const std::string& name = args_divided[2];
      if (storage_.count(id) > 0) throw InvalidArgument("id already taken");
      double energy;
      int score;
      try {
        energy = std::stod(args_divided[3]);
      } catch (const std::exception&) {
        throw InvalidArgument("energy is not a number");
      }
      try {
        score = std::stoi(args_divided[4]);
      } catch (const std::exception&) {
        throw InvalidArgument("score is not integer");
      }
      std::shared_ptr<ElectricBike> item =
          std::make_shared<ElectricBike>(id, name, energy, score);
      storage_[id] = item;
      last_added_.add(item);
      return "added " + id;
    } else if (args_divided[0] == "speedscooter") {
      if (args_divided.size() != 4)
        throw InvalidArgument("wrong arguments count for speedscooter");
      const std::string& id = args_divided[1];
      const std::string& name = args_divided[2];
      if (storage_.count(id) > 0) throw InvalidArgument("id already taken");
      double energy;
      try {
        energy = std::stod(args_divided[3]);
      } catch (const std::exception&) {
        throw InvalidArgument("energy is not a number");
      }
      std::shared_ptr<SpeedScooter> item =
          std::make_shared<SpeedScooter>(id, name, energy);
      storage_[id] = item;
      last_added_.add(item);
      return "added " + id;
    } else if (args_divided[0] == "helmet") {
      if (args_divided.size() != 3)
        throw InvalidArgument("wrong arguments count for helmet");
      const std::string& id = args_divided[1];
      const std::string& name = args_divided[2];
      if (storage_.count(id) > 0) throw InvalidArgument("id already taken");
      std::shared_ptr<Helmet> item = std::make_shared<Helmet>(id, name);
      storage_[id] = item;
      last_added_.add(item);
      return "added " + id;
    } else if (args_divided[0] == "dock") {
      if (args_divided.size() != 4)
        throw InvalidArgument("wrong arguments count for dock");
      const std::string& id = args_divided[1];
      const std::string& name = args_divided[2];
      if (storage_.count(id) > 0) throw InvalidArgument("id already taken");
      double energy;
      try {
        energy = std::stod(args_divided[3]);
      } catch (const std::exception&) {
        throw InvalidArgument("energy is not a number");
      }
      std::shared_ptr<DockStation> item =
          std::make_shared<DockStation>(id, name, energy);
      storage_[id] = item;
      last_added_.add(item);
      return "added " + id;
    } else {
      throw InvalidArgument("unknown item");
    }
  }
  std::string energy() {
    return "daily energy consumption: " + std::to_string(countEnergy());
  }
  std::string novice() {
    std::string res{};
    for (auto item : storage_) {
      int score = item.second->score();
      if (score >= 6) {
        res += item.second->describe() + ", ";
      }
    }
    if (res.empty()) {
      return "no transport for novice";
    }
    if (res.size() >= 2) {
      res.erase(res.size() - 2, 2);
    }
    return res;
  }
  std::string list() {
    std::string res{};
    std::vector<std::shared_ptr<InventoryItem>> sorted_storage;
    for (auto item : storage_) {
      sorted_storage.push_back(item.second);
    }
    std::sort(sorted_storage.begin(), sorted_storage.end(),
              [](const auto& a, const auto& b) { return *a < *b; });
    for (auto item : sorted_storage) {
      res += item->describe() + ", ";
    }
    if (res.empty()) {
      return "no items";
    }
    return res;
  }
  std::string focus(const std::string& id) {
    if (storage_.count(id) == 0) {
      throw InvalidArgument("item not found");
    }
    focus_ = storage_[id];
    return "focused " + storage_[id]->describe();
  }
  std::string show_focus() {
    if (focus_.expired()) {
      return "this item was removed";
    }
    return focus_.lock()->describe();
  }
  std::string remove(std::string id) {
    if (storage_.count(id) == 0) {
      throw InvalidArgument("item not found");
    }
    last_deleted_.add(storage_[id]->clone());
    std::string res = storage_[id]->describe();
    storage_.erase(id);
    return "removed " + res;
  }
  std::string last_added() {
    std::string res{};
    for (unsigned i = 0; i < last_added_.size(); ++i) {
      if (last_added_[i].expired()) {
        res += "removed item, ";
      } else {
        res += last_added_[i].lock()->describe() + ", ";
      }
    }
    if (res.size() >= 2) {
      res.erase(res.size() - 2, 2);
    }
    return res;
  }
  std::string last_deleted() {
    std::string res{};
    for (unsigned i = 0; i < last_deleted_.size(); ++i) {
      if (last_deleted_[i] != nullptr) {
        res += last_deleted_[i]->describe() + ", ";
      }
    }
    if (res.size() >= 2) {
      res.erase(res.size() - 2, 2);
    }
    return res;
  }
};