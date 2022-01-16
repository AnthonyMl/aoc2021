#include "aoc.hpp"
#include <regex>
#include <iostream>
#include <deque>
#include <array>

// part 1: 545118
// part 2: 1227298136842375

using IntType = int64_t;

struct Point {
    IntType x;
    IntType y;
    IntType z;
};

struct Range {
    Point start;
    Point end;

    IntType volume() const {
        return
            (1 + end.x - start.x) *
            (1 + end.y - start.y) *
            (1 + end.z - start.z);
    }
};

struct Command {
    bool on;
    Range range;
};

std::vector<Command> parse_parameters() {
    BaseData data("data22.txt");
    const std::regex expr{"(on|off) x=(-?[0-9]+)..(-?[0-9]+),y=(-?[0-9]+)..(-?[0-9]+),z=(-?[0-9]+)..(-?[0-9]+)"};
    std::smatch match;
    std::vector<Command> result;

    while(const auto &line = data.get_line()) {
        std::regex_match(*line, match, expr);

        result.push_back({
            match[1] == "on",
            std::stoi(match[2]),
            std::stoi(match[4]),
            std::stoi(match[6]),
            std::stoi(match[3]),
            std::stoi(match[5]),
            std::stoi(match[7]),
        });
    }
    return result;
}

std::optional<Range> intersect(const Range &a, const Range &b) {
    if( a.end.x < b.start.x || a.start.x > b.end.x ||
        a.end.y < b.start.y || a.start.y > b.end.y ||
        a.end.z < b.start.z || a.start.z > b.end.z) return {};

    std::array<IntType, 4> xs{a.start.x, a.end.x, b.start.x, b.end.x};
    std::array<IntType, 4> ys{a.start.y, a.end.y, b.start.y, b.end.y};
    std::array<IntType, 4> zs{a.start.z, a.end.z, b.start.z, b.end.z};

    std::sort(xs.begin(), xs.end());
    std::sort(ys.begin(), ys.end());
    std::sort(zs.begin(), zs.end());

    return {{ {xs[1], ys[1], zs[1]}, {xs[2], ys[2], zs[2]} }};
}

bool remove(const Command &cmd, IntType limit) {
    return
        cmd.range.start.x > limit ||
        cmd.range.start.y > limit ||
        cmd.range.start.z > limit ||
        cmd.range.end.x < -limit ||
        cmd.range.end.y < -limit ||
        cmd.range.end.z < -limit;
}

void limit_commands(std::vector<Command> &commands, IntType limit) {
    for(auto &cmd : commands) {
        cmd.range.start.x = std::max(cmd.range.start.x, -limit);
        cmd.range.start.y = std::max(cmd.range.start.y, -limit);
        cmd.range.start.z = std::max(cmd.range.start.z, -limit);
        cmd.range.end.x = std::min(cmd.range.end.x, limit);
        cmd.range.end.y = std::min(cmd.range.end.y, limit);
        cmd.range.end.z = std::min(cmd.range.end.z, limit);
    }
}

std::vector<Command> process_commands(const std::vector<Command> &cmds, IntType limit) {
    std::vector<Command> result;
    for(const auto &cmd : cmds) if(!remove(cmd, limit)) result.push_back(cmd);

    limit_commands(result, limit);

    return result;
}

void count_ons(std::optional<int> limit) {
    auto commands = parse_parameters();

    if(limit) commands = process_commands(commands, *limit);

    std::vector<Command> output;
    for(const auto &cmd : commands) {
        std::vector<Command> added;

        if(cmd.on) added.push_back(cmd);

        for(const auto &out : output) {
            if(auto intersection = intersect(cmd.range, out.range)) {
                added.push_back({!out.on, *intersection});
            }
        }
        for(const auto &a : added) output.push_back(a);
    }
    IntType on_count = 0;
    for(const auto &o : output) on_count += o.range.volume() * (o.on ? 1 : -1);
    std::cout << "on count: " << on_count << "\n";
}

void part1() {
    count_ons(50);
}

void part2() {
    count_ons({});
}

int main() {
    part1();

    part2();

    return 0;
}
