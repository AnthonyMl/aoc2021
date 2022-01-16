#include "aoc.hpp"
#include <iostream>
#include <regex>

// part1: 5778
// part2: 2576

struct Target {
    int left;
    int right;
    int bottom;
    int top;
};

Target parse_target() {
    const std::regex expr{"target area: x=(-?[0-9]+)..(-?[0-9]+), y=(-?[0-9]+)..(-?[0-9]+)"};
    std::smatch match;
    BaseData data("data17.txt");
    auto line = data.get_line();

    std::regex_match(*line, match, expr);

    return {
        std::stoi(match[1]),
        std::stoi(match[2]),
        std::stoi(match[3]),
        std::stoi(match[4]) };
}

struct Velocity {
    int dx;
    int dy;
};

void step(Point2 &s, Velocity &v) {
    s.x += v.dx;
    s.y += v.dy;
    if(v.dx < 0) v.dx++;
    else if(v.dx > 0) v.dx--;
    v.dy -= 1;
}

std::optional<int> simulate(const Target &t, Velocity v0) {
    Velocity v = v0;
    Point2 s{0, 0};
    int max_height = 0;

    while(true) {
        step(s, v);

        max_height = std::max(max_height, s.y);

        if(s.x > t.right || s.y < t.bottom) return {};
        if(s.x >= t.left && s.x <= t.right && s.y <= t.top && s.y >= t.bottom) {
            return max_height;
        }
    }
    throw std::out_of_range{"unreachable"};
}

void part1(const Target &target) {
    int max_height = 0;
    for(int dx = 0; dx < target.right; ++dx) {
        for(int dy = 0; dy < abs(target.bottom); ++dy) {
            auto height = simulate(target, {dx, dy});
            if(height) max_height = std::max(max_height, *height);
        }
    }
    std::cout << "max height = " << max_height << "\n";
}

void part2(const Target &target) {
    int good_count = 0;
    for(int dx = 0; dx < 2 * target.right; ++dx) {
        for(int dy = target.bottom; dy < 2 * abs(target.bottom); ++dy) {
            auto result = simulate(target, {dx, dy});
            if(result) good_count++;
        }
    }
    std::cout << "good v0s: " << good_count << "\n";
}

int main() {
    auto target = parse_target();

    part1(target);

    part2(target);

    return 0;
}
