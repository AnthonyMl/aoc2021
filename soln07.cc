#include "aoc.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <numeric>

// part 1: 325528
// part 2: 85015836

void minimum_cost(int cost_fn(int)) {
    BaseData data{"data07.txt"};
    std::vector<int> positions = line_to_ints(*data.get_line());
    int min_pos = positions[0];
    int max_pos = positions[0];
    for(auto &position : positions) {
        max_pos = std::max(max_pos, position);
        min_pos = std::min(min_pos, position);
    }
    int min_cost = std::numeric_limits<int>::max();
    int optimal_position;
    for(int i = min_pos; i < max_pos; ++i) {
        int cost = 0;
        for(auto &position: positions) {
            cost += cost_fn(abs(position - i));
        }
        if(cost < min_cost) {
            min_cost = cost;
            optimal_position = i;
        }
    }
    std::cout << "min_cost: " << min_cost << "\n";
}

int main() {
    minimum_cost([](int x){ return x; });

    minimum_cost([](int x){ return x * (x + 1) / 2; });

    return 0;
}
