#include "aoc.hpp"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>

// part 1: 364389
// part 2: 2870201088

void part1() {
    BaseData data("data10.txt");
    const std::unordered_map<char, char> open_close{
        {'(',')'}, {'[',']'}, {'{','}'}, {'<','>'} };
    const std::unordered_map<char, int> score{
        {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137} };
    unsigned int error_score = 0;

    while(auto line = data.get_line()) {
        std::vector<char> opened;

        for(const auto &c : *line) {
            if(open_close.contains(c)) {
                opened.push_back(c);
            } else {
                if(c == open_close.at(opened.back())) {
                    opened.pop_back();
                } else {
                    error_score += score.at(c);
                    break;
                }
            }
        }
    }
    std::cout << "Error score: " << error_score << "\n";
}

void part2() {
    BaseData data("data10.txt");
    const std::unordered_map<char, char> open_close{
        {'(',')'}, {'[',']'}, {'{','}'}, {'<','>'} };
    const std::unordered_map<char, int> score{
        {'(', 1}, {'[', 2}, {'{', 3}, {'<', 4} };

    std::vector<unsigned long long> scores;

    while(auto line = data.get_line()) {
        std::vector<char> opened;
        bool line_corrupted = false;

        for(const auto &c : *line) {
            if(open_close.contains(c)) {
                opened.push_back(c);
            } else {
                if(c == open_close.at(opened.back())) {
                    opened.pop_back();
                } else {
                    line_corrupted = true; // labeled continue?
                    break;
                }
            }
        }
        if(line_corrupted) continue;

        unsigned long long line_score = 0;
        for(auto it = opened.rbegin(); it != opened.rend(); ++it) {
            line_score *= 5;
            line_score += score.at(*it);
        }
        scores.push_back(line_score);
    }
    std::sort(scores.begin(), scores.end());

    std::cout << "Middle score: " << scores[scores.size() / 2] << "\n";
}

int main() {
    part1();

    part2();

    return 0;
}
