#include "lan_party.hpp"
#include "file_helpers.hpp"
#include <algorithm>
#include <cstdint>
#include <list>
#include <set>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

std::set<std::set<std::string>> get_computer_pairs(const std::string &filename) {
  auto file = open_file(filename);
  std::set<std::set<std::string>> computer_pairs;
  std::string line;
  while (std::getline(file, line)) {
    const std::string first = line.substr(0, 2);
    const std::string second = line.substr(3, 2);
    const std::set<std::string> pcs{first, second};
    computer_pairs.insert(pcs);
  }
  file.close();
  return computer_pairs;
}

struct PC {
  std::string name;
  std::set<std::string> connections;
  bool operator==(const PC &other) const { return this->name == other.name; }
  bool operator<(const PC &other) const { return this->name < other.name; }
};

using Graph = std::list<PC>;

std::vector<Graph> bron_kerbosh(Graph P, const Graph &R, Graph X) {
  std::vector<Graph> cliques;
  if ((P.empty() && X.empty())) {
    cliques.push_back(R);
    return cliques;
  }

  while (!P.empty()) {
    const auto v = P.front();
    Graph Ri = R;
    Graph Pi = P;
    Graph Xi = X;

    Ri.push_back(v);
    Pi.remove_if([&v](const PC &p) { return !v.connections.contains(p.name); });
    Xi.remove_if([&v](const PC &x) { return !v.connections.contains(x.name); });
    const auto new_cliques = bron_kerbosh(Pi, Ri, Xi);
    cliques.insert(cliques.end(), new_cliques.begin(), new_cliques.end());

    P.remove(v);
    X.push_back(v);
  }
  return cliques;
}

std::unordered_map<std::string, std::set<std::string>>
get_map_of_connections(const std::set<std::set<std::string>> &connections) {
  std::unordered_map<std::string, std::set<std::string>> map_of_connections;
  for (const auto &connection : connections) {
    const auto first = *connection.begin();
    const auto second = *std::next(connection.begin());
    map_of_connections[first].insert(second);
    map_of_connections[second].insert(first);
  }
  return map_of_connections;
}

std::vector<std::string> get_common_pcs(const std::set<std::string> &first, const std::set<std::string> &second) {
  std::vector<std::string> commonc_pcs;
  for (const auto &pc : first) {
    if (second.contains(pc)) {
      commonc_pcs.push_back(pc);
    }
  }
  return commonc_pcs;
}

bool one_of_pcs_starts_with_t(std::set<std::string> &connection) {
  return std::any_of(connection.begin(), connection.end(), [](const auto &pc) { return pc[0] == 't'; });
}

std::set<std::set<std::string>> get_connected_computers(const std::set<std::set<std::string>> &connected_pairs) {
  const auto map_of_connections = get_map_of_connections(connected_pairs);
  std::set<std::set<std::string>> sets_of_connected_cps;
  for (const auto &connection : connected_pairs) {
    const auto first_connections = map_of_connections.at(*connection.begin());
    const auto second_connections = map_of_connections.at(*std::next(connection.begin()));
    const auto common_pcs = get_common_pcs(first_connections, second_connections);
    for (const auto &pc : common_pcs) {
      auto new_connection = connection;
      new_connection.insert(pc);
      if (one_of_pcs_starts_with_t(new_connection)) {
        sets_of_connected_cps.insert(new_connection);
      }
    }
  }
  return sets_of_connected_cps;
}

Graph get_list_of_pcs(const std::unordered_map<std::string, std::set<std::string>> &pcs_map) {
  Graph pcs;
  for (const auto &[id, connections] : pcs_map) {
    pcs.push_back({id, connections});
  }
  pcs.sort();
  return pcs;
}

std::string transform_graph_to_str(const Graph &pcs) {
  std::string graph_str;
  for (const auto &pc : pcs) {
    graph_str += pc.name + ',';
  }
  graph_str.erase(std::prev(graph_str.end()));
  return graph_str;
}

uint64_t get_num_of_connections(const Graph &pcs) {
  const auto connections = bron_kerbosh(pcs, {}, {});
  return static_cast<uint64_t>(std::count_if(connections.begin(), connections.end(),
                                             [](const auto &connection) { return connection.size() == 3ull; }));
}

std::string get_biggest_party(const Graph &pcs) {
  const auto parties = bron_kerbosh(pcs, {}, {});
  auto biggest_party = *std::max_element(parties.begin(), parties.end(),
                                         [](const auto &a, const auto &b) { return a.size() < b.size(); });
  biggest_party.sort();
  return transform_graph_to_str(biggest_party);
}

std::variant<uint64_t, std::string> solve(const bool part_2) {
  const auto computer_pairs = get_computer_pairs("data.txt");
  if (!part_2) {
    const auto connected_computers = get_connected_computers(computer_pairs);
    return connected_computers.size();
  }
  const auto map_of_connections = get_map_of_connections(computer_pairs);
  const auto pcs = get_list_of_pcs(map_of_connections);
  return get_biggest_party(pcs);
}
