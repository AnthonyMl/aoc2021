#pragma once
#ifndef AOC_HPP
#define AOC_HPP

#include <fstream>
#include <optional>
#include <string>
#include <vector>
#include <regex>

struct BaseData {
protected:
    std::ifstream stream;

public:
    BaseData(const char *filename) {
        stream = std::ifstream(filename);
        if(!stream.is_open()) {
            throw std::invalid_argument("Unable to open data file");
        }
    }

    std::optional<std::string> get_line() {
        std::string buffer;
        std::getline(stream, buffer);
        if(stream.eof()) return {};
        return buffer;
    }
};

std::vector<int> line_to_ints(const std::string &line) {
    const std::regex expr("([0-9]+)");

    std::vector<int> result;
    std::transform(
        std::sregex_token_iterator{line.begin(), line.end(), expr},
        std::sregex_token_iterator{},
        std::back_inserter(result),
        [](std::string const &s){ return std::stoi(s); });
    return result;
}

std::vector<int> line_to_digits(const std::string &line) {
    const std::regex expr("([0-9])");

    std::vector<int> result;
    std::transform(
        std::sregex_token_iterator{line.begin(), line.end(), expr},
        std::sregex_token_iterator{},
        std::back_inserter(result),
        [](std::string const &s){ return std::stoi(s); });
    return result;
}

struct int_int_hash {
    std::size_t operator()(const std::pair<int, int> &p) const {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
    }
};

struct Point2 {
    int x;
    int y;

    bool operator==(const Point2 p) const { return x == p.x && y == p.y; }
    bool operator!=(const Point2 p) const { return x != p.x || y != p.y; }
    Point2 operator+(const Point2 &p) const { return {x + p.x, y + p.y}; }
};

struct point2_hash {
    std::size_t operator()(const Point2 &p) const {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
    }
};


#endif
