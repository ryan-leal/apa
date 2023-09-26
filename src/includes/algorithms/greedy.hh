#pragma once

#include "algorithm.hh"

namespace apa {

class greedy : public algorithm {
 public:
  explicit greedy(const apa::context &instance) : algorithm("greedy", instance) {}

  [[nodiscard]] apa::stats run() override;

 protected:
  [[nodiscard]] int find_highest_priority_client(int origin, int vehicle);
};

}  // namespace apa
