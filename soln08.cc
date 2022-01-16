#include "aoc.hpp"
#include <iostream>
#include <vector>
#include <regex>
#include <array>
#include <unordered_map>
#include <unordered_set>

// part 1: 479
// part 2: 1041746

namespace Repr {
    const int digit_count = 10;
    const int segment_count = 7;
    const int display_count = 4;

    constexpr std::array<std::string_view, digit_count> str
        {"abcefg", "cf", "acdeg", "acdfg", "bcdf", "abdfg", "abdefg", "acf", "abcdefg", "abcdfg"};
}

class Data : BaseData {
public:
    Data(const char *filename): BaseData(filename) {}

    std::optional<std::vector<std::string>> numbers() {
        if(const auto &line = get_line()) {
            const std::regex expr{"([a-g]+)"};
            return {{
                std::sregex_token_iterator{line->begin(), line->end(), expr},
                std::sregex_token_iterator{}}};
        }
        return {};
    }
};

void part1() {
    Data data("data08.txt");
    int unique_count = 0;

    while(auto numbers = data.numbers()) {
        for(auto it = numbers->rbegin(); it != numbers->rbegin() + Repr::display_count; ++it) {
            switch(it->length()) {
                case Repr::str[1].size():
                case Repr::str[4].size():
                case Repr::str[7].size():
                case Repr::str[8].size():
                    unique_count++;
                    break;
            }
        }
    }
    std::cout << "unique outputs: " << unique_count << "\n";
}

// assumes a.length() > b.length()
int distance(const std::string &a, const std::string &b) {
    int same = 0;
    for(const auto &b_char : b) {
        if(std::find(a.begin(), a.end(), b_char) != a.end()) same++;
    }
    return a.length() - same;
}

bool contains_all(const std::string &a, const std::string &b) {
    for(const auto &b_char : b) {
        if(std::find(a.begin(), a.end(), b_char) == a.end()) return false;
    }
    return true;
}

void part2() {
    Data data("data08.txt");
    uint64_t sum = 0;

    while(auto numbers = data.numbers()) {
        std::unordered_map<int, std::unordered_set<std::string>> length_representations;
        std::unordered_map<std::string, int> representation_number;
        std::unordered_map<int, std::string> number_representation; // TODO: use a bimap

        for(auto &s : *numbers) {
            std::sort(s.begin(), s.end());
            const int length = s.length();
            length_representations[length].insert(s);
            const std::array<int, 4> initial_known_set{1, 4, 7, 8};

            for(const auto &n : initial_known_set) {
                if(length == Repr::str[n].length()) {
                    representation_number[s] = n;
                    number_representation[n] = s;
                }
            }
        }
        // The only 5 length number that contains 1's segments is 3
        for(const auto &possibly_three : length_representations[5]) {
            if(contains_all(possibly_three, number_representation[1])) {
                representation_number[possibly_three] = 3;
                number_representation[3] = possibly_three;
                length_representations[5].erase(possibly_three);
                break; // needed because the iterator broke on the previous line
            }
        }
        // The only 6 length string that does NOT contain both of 1's segments is 6
        for(const auto &possibly_six : length_representations[6]) {
            if(!contains_all(possibly_six, number_representation[1])) {
                representation_number[possibly_six] = 6;
                number_representation[6] = possibly_six;
                length_representations[6].erase(possibly_six);
                break;
            }
        }
        // 6 is 1 away from 5 and 2 away from 2 (the unknown, length 5 numbers)
        for(const auto &five_or_two : length_representations[5]) {
            int d = distance(number_representation[6], five_or_two);
            if(d == 1) {
                representation_number[five_or_two] = 5;
            } else if(d == 2) {
                representation_number[five_or_two] = 2;
            }
        }
        // 3 is 1 away from 9 and 2 away from 0 (the unknown, length 6 numbers)
        for(const auto &zero_or_nine : length_representations[6]) {
            int d = distance(zero_or_nine, number_representation[3]);
            if(d == 1) {
                representation_number[zero_or_nine] = 9;
            } else if(d == 2) {
                representation_number[zero_or_nine] = 0;
            }
        }

        int line_total = 0;
        int factor = 1;
        for(int i = 0; i < Repr::display_count; ++i) {
            line_total += factor * representation_number[*(numbers->rbegin() + i)];
            factor *= 10;
        }
        sum += line_total;
    }
    std::cout << "sum = " << sum << "\n";
}

int main() {
    part1();

    part2();

    return 0;
}
