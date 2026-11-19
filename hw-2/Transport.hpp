#include <string>

#include "Inventory.hpp"

class Transport : public InventoryItem {
 protected:
  int score_;

 public:
  Transport(std::string id, std::string name, int score)
      : InventoryItem(id, name), score_(score) {
    if (score < 1 || score > 10) {
      throw InvalidArgument("wrong score");
    }
    if (toLower(name).find("broken") != std::string::npos) {
      throw InspectionFailed("item is broken");
    }
  }
  std::string inventoryId() const override { return id_; }
  std::string name() const override { return name_; }
  std::string describe() const override { return id_ + " " + name_; }
  int score() const override { return score_; }
};

class Bike : public Transport {
 public:
  Bike(std::string id, std::string name, int score)
      : Transport(id, name, score) {};
  std::unique_ptr<InventoryItem> clone() const override {
    return std::make_unique<Bike>(*this);
  }
};

class Scooter : public Transport, public EnergyConsumer {
 protected:
  double energy_;

 public:
  Scooter(std::string id, std::string name, double energy, int score)
      : Transport(id, name, score), energy_(energy) {
    if (energy < 0) {
      throw InvalidArgument("negative energy");
    }
  };
  double dailyEnergy() const override { return energy_; }
  double getEnergy() const override { return dailyEnergy(); }
  std::unique_ptr<InventoryItem> clone() const override {
    return std::make_unique<Scooter>(*this);
  }
};

class ElectricBike : public Transport, public EnergyConsumer {
 protected:
  double energy_;

 public:
  ElectricBike(std::string id, std::string name, double energy, int score)
      : Transport(id, name, score), energy_(energy) {
    if (energy < 0) {
      throw InvalidArgument("negative energy");
    }
  };
  double dailyEnergy() const override { return energy_; }
  double getEnergy() const override { return dailyEnergy(); }
  std::unique_ptr<InventoryItem> clone() const override {
    return std::make_unique<ElectricBike>(*this);
  }
};

class SpeedScooter : public Transport, public EnergyConsumer {
 protected:
  double energy_;

 public:
  SpeedScooter(std::string id, std::string name, double energy)
      : Transport(id, name, 1), energy_(energy) {
    if (energy < 0) {
      throw InvalidArgument("negative energy");
    }
  };
  double dailyEnergy() const override { return energy_; }
  double getEnergy() const override { return dailyEnergy(); }
  std::unique_ptr<InventoryItem> clone() const override {
    return std::make_unique<SpeedScooter>(*this);
  }
};