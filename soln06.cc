#include "aoc.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <array>

// part 1: 362740
// part 2: 1644874076764

std::vector<int> parse_initial_ages(const std::string &line) {
    std::regex expr("([0-9]+)");
    std::vector<int> result;

    std::transform(
        std::sregex_token_iterator{line.begin(), line.end(), expr},
        std::sregex_token_iterator{},
        std::back_inserter(result),
        [](std::string const &s){ return std::stoi(s); });
    return result;
}

void simulate(int simulation_days) {
    const int max_age = 9;
    BaseData data("data06.txt");
    auto initial_ages = parse_initial_ages(*data.get_line());

    std::array<uint64_t, max_age> ages = {};
    for(auto age : initial_ages) ages[age]++;

    for(int day = 0; day < simulation_days; ++day) {
        std::array<uint64_t, max_age> new_ages;
        for(int i = 0; i < max_age - 1; ++i) new_ages[i] = ages[i + 1];

        new_ages[6] += ages[0]; // cycle back
        new_ages[8] = ages[0];

        std::swap(ages, new_ages);
    }
    uint64_t total_fish = 0;
    for(const auto &age : ages) total_fish += age;
    std::cout << "total fish: " << total_fish << "\n";
}


int main() {
    simulate(80);

    simulate(256);

    return 0;
}
