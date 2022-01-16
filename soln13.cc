#include "aoc.hpp"
#include <unordered_set>
#include <regex>
#include <iostream>

// part 1: 666
// part 2: CJHAZHKU

using PointSet = std::unordered_set<std::pair<int, int>, int_int_hash>;

PointSet parse_points(BaseData &data) {
    PointSet points;

    while(const auto &line = data.get_line()) {
        const std::regex expr{"([0-9]+),([0-9]+)"};
        std::smatch match;

        if(!std::regex_match(*line, match, expr)) break;

        points.insert({std::stoi(match[1]), std::stoi(match[2])});
    }
    return points;
}

enum class Axis { x, y };

struct Fold {
    Axis axis;
    int coord;
};

std::vector<Fold> parse_folds(BaseData &data) {
    const std::regex expr{"fold along ([x-y])=([0-9]+)"};
    std::smatch match;
    std::vector<Fold> folds;

    while(const auto &line = data.get_line()) {
        if(!std::regex_match(*line, match, expr)) break;

        folds.push_back({match[1] == "x" ? Axis::x : Axis::y, std::stoi(match[2])});
    }
    return folds;
}

void part1(const PointSet &points, const Fold &f) {
    int overlap_count = 0;
    switch(f.axis) {
    case Axis::x:
        for(const auto &[x, y] : points) {
            if(x > f.coord && points.find({2 * f.coord - x, y}) != points.end()) {
                overlap_count++;
            }
        }
        break;
    case Axis::y:
        for(const auto &[x, y] : points) {
            if(y > f.coord && points.find({x, 2 * f.coord - y}) != points.end()) {
                overlap_count++;
            }
        }
        break;
    }
    std::cout << "visible points: " << points.size() - overlap_count << "\n";
}

void perform_folds(PointSet &points, const std::vector<Fold> &folds) {
    for(const auto &f : folds) {
        switch(f.axis) {
        case Axis::x:
            for(const auto &[x, y] : points) {
                if(x > f.coord) points.insert({2 * f.coord - x, y});
            }
            std::erase_if(points, [&f](const auto &p){ return p.first > f.coord; });
            break;
        case Axis::y:
            for(const auto &[x, y] : points) {
                if(y > f.coord) points.insert({x, 2 * f.coord - y});
            }
            std::erase_if(points, [&f](const auto &p){ return p.second > f.coord; });
            break;
        }
    }
}

void part2(PointSet &points, const std::vector<Fold> &folds) {
    perform_folds(points, folds);

    int max_x = 0;
    int max_y = 0;
    for(const auto &[x, y] : points) {
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
    }
    for(int y = 0; y <= max_y; ++y) {
        for(int x = 0; x <= max_x; ++x) {
            std::cout << (points.find({x, y}) != points.end() ? '#' : '.');
        }
        std::cout << "\n";
    }
}

int main() {
    BaseData data("data13.txt");

    auto points = parse_points(data);

    auto folds = parse_folds(data);

    part1(points, folds[0]);

    part2(points, folds);

    return 0;
}
