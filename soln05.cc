#include "aoc.hpp"
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <regex>

// part 1: 7417
// part 2: 19676

std::tuple<int, int, int, int> parse_line(const std::string &line) {
    std::regex expr("([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+)");
    std::smatch match;

    std::regex_match(line, match, expr);
    return {
        std::stoi(match[1].str()),
        std::stoi(match[2].str()),
        std::stoi(match[3].str()),
        std::stoi(match[4].str())
    };
}

using Frequencies = std::unordered_map<std::pair<int, int>, int, int_int_hash>;

void part1() {
    BaseData data("data05.txt");
    Frequencies frequencies;

    while(auto str_line = data.get_line()) {
        auto [x1, y1, x2, y2] = parse_line(*str_line);

        if(x1 == x2) {
            if(y1 > y2) std::swap(y1, y2);
            for(;y1 <= y2; ++y1) frequencies[{x1, y1}]++;
        } else if(y1 == y2) {
            if(x1 > x2) std::swap(x1, x2);
            for(;x1 <= x2; ++x1) frequencies[{x1, y1}]++;
        }
    }
    int double_overlap_count = 0;
    for(auto &[p_, frequency] : frequencies) {
        if(frequency > 1) double_overlap_count++;
    }
    std::cout << "double overlap (straight lines): " << double_overlap_count << "\n";
}

void part2() {
    BaseData data("data05.txt");
    Frequencies frequencies;

    while(auto str_line = data.get_line()) {
        auto [x1, y1, x2, y2] = parse_line(*str_line);

        if(x1 == x2) {
            if(y1 > y2) std::swap(y1, y2);
            for(;y1 <= y2; ++y1) frequencies[{x1, y1}]++;
        } else if(y1 == y2) {
            if(x1 > x2) std::swap(x1, x2);
            for(;x1 <= x2; ++x1) frequencies[{x1, y1}]++;
        } else {
            if(x1 > x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            for(;x1 <= x2; ++x1) {
                frequencies[{x1, y1}]++;
                if(y1 > y2) y1--;
                else        y1++;
            }
        }
    }
    int double_overlap_count = 0;
    for(auto &[p_, frequency] : frequencies) {
        if(frequency > 1) double_overlap_count++;
    }
    std::cout << "double overlap (allow diagonals): " << double_overlap_count << "\n";
}

int main() {
    part1();

    part2();

    return 0;
}
