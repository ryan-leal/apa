#include "../includes/algorithms/greedy.hh"

#include <algorithm>
#include <iostream>
#include <limits>

namespace apa {

apa::stats greedy::run() {
  int routing_costs{};
  int outsourcing_costs{};

  std::unordered_set<int> pending_clients{};
  std::vector<std::vector<int>> vehicle_routes(context().vehicles, std::vector<int>());
  std::vector<int> outsourced_clients{};
  std::vector<int> vehicle_capacity(context().vehicles, context().vehicle_capacity);

  for (int client = 6; client >= 1; client--) {
    pending_clients.insert(client);
  }

  for (int vehicle = 0; vehicle < context().vehicles; vehicle++) {
    int origin{0};  // warehouse

    while (!pending_clients.empty() && vehicle_capacity[vehicle] > 0) {
      int client{find_next_client(origin, vehicle, pending_clients, vehicle_capacity)};
      if (client == -1) {
        // Can't find a client to serve with the remaining capacity of the current vehicle.
        // So, we go back to the warehouse.
        routing_costs += context().distance(origin, 0);

        //        std::cout << "V" << vehicle << ": "
        //                  << "[" << origin << "] -> [" << 0 << "] | "
        //                  << "demand: " << 0 << " | cost: " << context().distance(origin, 0) <<
        //                  std::endl;
        break;
      }

      int demand{context().demand(client)};
      int cost{context().distance(origin, client)};

      vehicle_capacity[vehicle] -= demand;
      vehicle_routes[vehicle].push_back(client);

      pending_clients.erase(client);

      //      std::cout << "V" << vehicle << ": "
      //                << "[" << origin << "] -> [" << client << "] | "
      //                << "demand: " << demand << " | cost: " << cost << std::endl;

      routing_costs += cost;
      origin = client;

      if (vehicle_capacity[vehicle] == 0) {
        // The current vehicle is full, so we go back to the warehouse.
        routing_costs += context().distance(origin, 0);

        //        std::cout << "V" << vehicle << ": "
        //                  << "[" << origin << "] -> [" << 0 << "] | "
        //                  << "demand: " << 0 << " | cost: " << context().distance(origin, 0) <<
        //                  std::endl;
      }
    }
  }

  // outsource remaining clients
  for (const auto &client : pending_clients) {
    outsourcing_costs += context().outsourcing_cost(client);
    outsourced_clients.push_back(client);
  }

  int vehicle_costs = static_cast<int>(
      std::count_if(vehicle_capacity.begin(),
                    vehicle_capacity.end(),
                    [&](int capacity) { return capacity < context().vehicle_capacity; }) *
      context().vehicle_cost);

  return {
      routing_costs + vehicle_costs + outsourcing_costs,  // total_cost
      routing_costs,                                      // routing_cost
      vehicle_costs,                                      // vehicles_cost
      outsourcing_costs,                                  // outsourcing_cost
      outsourced_clients,                                 // outsourced_clients
      vehicle_routes                                      // vehicle_routes
  };
}

int greedy::find_next_client(int origin,
                             int vehicle,
                             const std::unordered_set<int> &clients,
                             const std::vector<int> &vehicle_capacity) {
  int next_client{-1};
  int next_client_cost{std::numeric_limits<int>::max()};

  // can we improve the greedy criteria?

  for (const auto &client : clients) {
    int demand{context().demand(client)};
    int distance{context().distance(origin, client)};

    int cost{distance};

    if (cost < next_client_cost && demand <= vehicle_capacity[vehicle]) {
      next_client = client;
      next_client_cost = distance;
    }
  }

  return next_client;
}

}  // namespace apa