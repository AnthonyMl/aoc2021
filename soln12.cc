#include "aoc.hpp"
#include <regex>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

// part 1: 3292
// part 2: 89592

using Graph = std::unordered_map<std::string, std::unordered_set<std::string>>;

int count_paths(const Graph &g, const std::string &node, std::unordered_set<std::string> visited, bool doubled) {
    if(node == "end") return 1;
    if(islower(node[0]) && visited.find(node) != visited.end()) {
        if(doubled) return 0;
        doubled = true;
    }

    if(islower(node[0])) visited.insert(node);

    int path_count = 0;
    for(const auto &neighbor : g.at(node)) {
        path_count += count_paths(g, neighbor, visited, doubled);
    }
    return path_count;
}

void part1(const Graph &g) {
    std::cout << "path count 1: " << count_paths(g, "start", {}, true) << "\n";
}

void part2(const Graph &g) {
    std::cout << "path count 2: " << count_paths(g, "start", {}, false) << "\n";
}

int main() {
    BaseData data("data12.txt");

    Graph g;
    while(const auto &line = data.get_line()) {
        const std::regex expr{"([A-z]+)-([A-z]+)"};
        std::smatch match;

        std::regex_match(*line, match, expr);

        if(match[2] != "start") g[match[1]].insert(match[2]);
        if(match[1] != "start") g[match[2]].insert(match[1]);
    }

    part1(g);

    part2(g);

    return 0;
}
