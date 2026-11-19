#pragma once

#include <cctype>
#include <stdexcept>
#include <string>

class FleetError : public std::runtime_error {
 public:
  FleetError(const std::string& msg) : std::runtime_error(msg) {}
};

class InspectionFailed : public FleetError {
 public:
  InspectionFailed(const std::string& msg) : FleetError(msg) {}
};

class InvalidArgument : public FleetError {
 public:
  InvalidArgument(const std::string& msg) : FleetError(msg) {}
};

inline std::string toLower(std::string s) {
  for (char& c : s) {
    c = std::tolower(c);
  }
  return s;
}