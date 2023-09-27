#include "../includes/algorithms/greedy.hh"

#include <iostream>
#include <limits>

namespace apa {

apa::stats greedy::run() {
  while (greedy::has_pending_clients()) {
    const int vehicle{greedy::find_vehicle_with_most_capacity()};
    const int origin_client{greedy::find_last_client_in_route(vehicle)};
    const int target_client{greedy::find_highest_priority_client(origin_client, vehicle)};

    if (target_client) {
      const int target_demand{context().demand(target_client)};
      const int target_distance{context().distance(origin_client, target_client)};

      greedy::load_vehicle(vehicle, target_demand);
      greedy::add_to_route(vehicle, target_client, target_distance);
      greedy::serve_client(target_client);
    } else {
      greedy::outsource_client(greedy::find_pending_client_with_lowest_outsource_cost());
    }
  }

  // All clients have been served, return to depot.
  for (int vehicle = 0; vehicle < context().vehicles; vehicle++) {
    greedy::return_to_depot(vehicle);
  }

  return {
      greedy::total_cost(),              // total_cost
      greedy::total_routing_cost(),      // routing_cost
      greedy::total_vehicle_cost(),      // vehicles_cost
      greedy::total_outsourcing_cost(),  // outsourcing_cost
      greedy::outsourced_clients(),      // outsourced_clients
      greedy::vehicle_routes()           // vehicle_routes
  };
}

int greedy::find_highest_priority_client(int origin, int vehicle) {
  int client{0};
  int client_priority{std::numeric_limits<int>::min()};

  for (const auto &target : pending_clients()) {
    const int target_demand{context().demand(target)};
    const int target_distance{context().distance(origin, target)};

    // TODO: this is a very naive approach, we should consider more than this. Also, outsource cost
    // is not being considered.
    const int priority{target_demand / target_distance};

    if (priority > client_priority && target_demand <= greedy::vehicle_capacity(vehicle)) {
      client = target;
      client_priority = target_distance;
    }
  }

  return client;
}

}  // namespace apa