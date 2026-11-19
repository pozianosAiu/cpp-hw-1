#include <string>

#include "Inventory.hpp"

class Helmet : public InventoryItem {
 public:
  Helmet(std::string id, std::string name) : InventoryItem(id, name) {}
  std::string inventoryId() const override { return id_; }
  std::string name() const override { return name_; }
  std::string describe() const override { return id_ + " " + name_; }
  std::unique_ptr<InventoryItem> clone() const override {
    return std::make_unique<Helmet>(*this);
  }
};

class DockStation : public InventoryItem, public EnergyConsumer {
 protected:
  double energy_;

 public:
  DockStation(std::string id, std::string name, double energy)
      : InventoryItem(id, name), energy_(energy) {
    if (energy < 0) {
      throw InvalidArgument("negative energy");
    }
  }
  std::string inventoryId() const override { return id_; }
  std::string name() const override { return name_; }
  std::string describe() const override { return id_ + " " + name_; }
  double dailyEnergy() const override { return energy_; }
  double getEnergy() const override { return dailyEnergy(); }
  std::unique_ptr<InventoryItem> clone() const override {
    return std::make_unique<DockStation>(*this);
  }
};
