#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "Errors.hpp"

class InventoryItem {
 protected:
  std::string id_;
  std::string name_;

 public:
  InventoryItem(std::string id, std::string name) : id_(id), name_(name) {
    if (id == "") {
      throw InvalidArgument("empty id");
    }
    if (name == "") {
      throw InvalidArgument("empty name");
    }
  }
  virtual std::string inventoryId() const = 0;
  virtual std::string name() const = 0;
  virtual std::string describe() const = 0;
  bool operator<(const InventoryItem& other) const {
    return inventoryId() < other.inventoryId();
  }
  bool operator==(const InventoryItem& other) const {
    return inventoryId() == other.inventoryId();
  }
  friend std::ostream& operator<<(std::ostream& out,
                                  const InventoryItem& item) {
    out << item.describe();
    return out;
  }
  virtual double getEnergy() const { return 0.0; }
  virtual int score() const { return -1; }
  virtual std::unique_ptr<InventoryItem> clone() const = 0;
  virtual ~InventoryItem() = default;
};

class EnergyConsumer {
 public:
  virtual double dailyEnergy() const = 0;
  virtual ~EnergyConsumer() = default;
};