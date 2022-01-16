#include "aoc.hpp"
#include <iostream>
#include <assert.h>

// part 1: 4132
// part 2: 4685

using Number = std::string;

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

// [start, end)
// ...last...[left,right]...next...
std::optional<Number> explode(const Number &n) {
    int left_start = 0;
    int last_end = 0;
    for(int bracket_count = 5; left_start < n.size(); ++left_start) {
        if(bracket_count == 0) break;

        if(is_digit(n[left_start])) {
            last_end = left_start;
        } else {
            if(n[left_start] == '[') bracket_count--;
            if(n[left_start] == ']') bracket_count++;
        }
    }
    if(left_start == n.size()) return {};

    int left_end = left_start + 1;
    for(; left_end > 0; ++left_end) if(!is_digit(n[left_end])) break;

    Number r;
    if(last_end == 0) {
        r = n.substr(0, left_start - 1);
        r += '0';
    } else {
        int last_start = last_end - 1;
        for(; last_start > 0; --last_start) if(!is_digit(n[last_start])) break;
        last_start++;
        last_end++;

        r = n.substr(0, last_start);
        int last = std::stoi(n.substr(last_start, last_end));
        int left = std::stoi(n.substr(left_start, left_end));
        r += std::to_string(last + left);
        r += n.substr(last_end, left_start - last_end - 1);
        r += '0';
    }

    int right_start = left_end + 1;
    int right_end = right_start + 1;
    for(; right_end < n.size(); ++right_end) if(!is_digit(n[right_end])) break;

    int next_start = right_end + 1;
    for(; next_start < n.size(); ++next_start) if(is_digit(n[next_start])) break;

    if(next_start < n.size()) {
        int next_end = next_start + 1;
        for(; next_end < n.size(); ++next_end) if(!is_digit(n[next_end])) break;

        r += n.substr(right_end + 1, next_start - right_end - 1);

        int right = std::stoi(n.substr(right_start, right_end - right_start));
        int next = std::stoi(n.substr(next_start, next_end - next_start));
        r += std::to_string(right + next);
        r += n.substr(next_end);
    } else {
        r += n.substr(right_end + 1);
    }
    return r;
}

std::optional<Number> split(const Number &n) {
    int end = 0;
    int length = 0;
    for(; end < n.size(); ++end) {
        if(is_digit(n[end])) length++;
        else if(length > 1) break;
        else length = 0;
    }
    if(end == n.size()) return {};

    Number r = n.substr(0, end - length);
    int sum = std::stoi(n.substr(end - length, length));
    r += "[";
    r += std::to_string(sum / 2);
    r += ",";
    r += std::to_string((sum + 1) / 2);
    r += "]";
    r += n.substr(end);
    return r;
}

Number reduce(Number n) {
    while(true) {
        if(auto n_new = explode(n)) {
            n = *n_new;
            continue;
        }
        if(auto n_new = split(n)) {
            n = *n_new;
            continue;
        }
        return n;
    }
    throw std::out_of_range("!unreachable(reduce)");
}

bool composite(const Number &n) {
    return n[0] == '[';
}

// assumes a comma will be found
int find_comma(const Number &n) {
    int bracket_count = 0;
    for(int i = 0; i < n.size(); ++i) {
        char c = n[i+1];
        if(c == '[') bracket_count++;
        if(c == ']') bracket_count--;
        if(c == ',' && bracket_count == 0) return i;
    }
    throw std::out_of_range("!unreachable(find_comma)");
}

// assumes reduced n
int magnitude(const Number &n) {
    if(composite(n)) {
        auto comma = find_comma(n);
        auto a = magnitude(n.substr(1, comma));
        auto b = magnitude(n.substr(comma + 2));;
        return 3 * a + 2 * b;
    } else {
        return std::stoi(n);
    }
    throw std::out_of_range("!unreachable(magnitude)");
}

Number add(const Number &n1, const Number &n2) {
    return reduce("[" + n1 + "," + n2 + "]");
}

void part1() {
    BaseData data("data18.txt");

    if(auto first_line = data.get_line()) {
        Number n1 = *first_line;
        while(const auto &line = data.get_line()) { // no ref?
            n1 = add(n1, *line);
        }
        std::cout << "magnitude: " << magnitude(n1) << "\n";
    }
}

void part2() {
    BaseData data("data18.txt");

    std::vector<Number> numbers;

    while(const auto &line = data.get_line()) numbers.push_back(*line);

    int max = 0;
    for(auto a = numbers.begin(); a != numbers.end(); ++a) {
        for(auto b = a + 1; b != numbers.end(); ++b) {
            max = std::max(max, magnitude(add(*a, *b)));
            max = std::max(max, magnitude(add(*b, *a)));
        }
    }
    std::cout << "max magnitude: " << max << "\n";
}

int main() {
    part1();

    part2();

    return 0;
}
