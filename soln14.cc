#include "aoc.hpp"
#include <regex>
#include <iostream>
#include <unordered_map>
#include <limits>

// part 1: 2587
// part 2: 3318837563123

using IntType = uint64_t;
using Frequency = std::unordered_map<char, IntType>;
using Rules = std::unordered_map<std::string, char>;

std::pair<std::string, Rules> parse_input() {
    BaseData data("data14.txt");
    std::string input{*data.get_line()};
    Rules rules;

    while(auto line = data.get_line()) {
        if(line->empty()) continue;

        const std::regex expr{"([A-Z]+) -> ([A-Z])"};
        std::smatch match;
        std::regex_match(*line, match, expr);

        rules[match[1].str()] = match[2].str()[0];
    }
    return { input, rules };
}

struct str_int_hash {
    std::size_t operator()(const std::pair<std::string, int> &p) const {
        return std::hash<std::string>()(p.first) ^ std::hash<int>()(p.second);
    }
};

using Cache = std::unordered_map<std::pair<std::string, int>, Frequency, str_int_hash>;

Frequency process_segment(std::string segment, int depth, const Rules &rules, Cache &cache) {
    if(depth == 0) {
        Frequency f;
        f[segment[0]]++;
        f[segment[1]]++;
        return f;
    }
    {
        auto entry = cache.find({segment, depth});
        if(entry != cache.end()) return entry->second;
    }
    auto rule = rules.at(segment); // assumption: every segment has an expansion rule

    std::string a{segment};
    std::string b{segment};

    a[1] = rule;
    b[0] = rule;

    Frequency f_a = process_segment(a, depth - 1, rules, cache);
    const Frequency f_b = process_segment(b, depth - 1, rules, cache);

    for(const auto &[c, freq]: f_b) f_a[c] += freq;

    f_a[rule]--; // compensate for double counting

    cache[{segment, depth}] = f_a;

    return f_a;
}

void largest_frequency_difference(int step_count) {
    const auto [input, rules] = parse_input();

    Cache cache;
    Frequency frequencies;
    for(int i = 0; i < input.length() - 1; ++i) {
        const auto fs = process_segment(input.substr(i, 2), step_count, rules, cache);

        for(const auto &[c, f]: fs) frequencies[c] += f;

        // compensate double counting only for inbetween entries
        if(i != input.length() - 2) frequencies[input[i + 1]]--;
    }

    uint64_t min = std::numeric_limits<uint64_t>::max();
    uint64_t max = 0;
    for(const auto &[c_, f] : frequencies) {
        min = std::min(min, f);
        max = std::max(max, f);
    }
    std::cout << "largest frequency difference: " << max - min << "\n";
}

void part1() {
    largest_frequency_difference(10);
}

void part2() {
    largest_frequency_difference(40);
}

int main() {
    part1();

    part2();

    return 0;
}
