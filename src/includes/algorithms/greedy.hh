#pragma once

#include <unordered_set>
#include <vector>

#include "algorithm.hh"

namespace apa {

class greedy : public algorithm {
 public:
  explicit greedy(const apa::context &instance) : algorithm("greedy", instance) {}

  [[nodiscard]] apa::stats run() override;

 protected:
  [[nodiscard]] int find_next_client(int origin,
                                     int vehicle,
                                     const std::unordered_set<int> &clients,
                                     const std::vector<int> &vehicle_capacity);
};

}  // namespace apa
