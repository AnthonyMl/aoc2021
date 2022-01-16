#include "aoc.hpp"
#include <vector>
#include <optional>
#include <regex>
#include <iostream>

// part 1: 518
// part 2: 949905

template <typename T> using PaddedGrid = std::vector<std::vector<T>>;

class Data: public BaseData {
public:
    Data(const char *filename): BaseData(filename) {}

    PaddedGrid<int> get_grid(int pad) {
        PaddedGrid<int> result;

        if(const auto &line1 = get_digits(pad)) {
            result.push_back(std::vector<int>(line1->size(), pad)); // top pad

            result.push_back(*line1);

            while(const auto &line = get_digits(pad)) result.push_back(*line);

            result.push_back(std::vector<int>(line1->size(), pad)); // bottom pad

            return result;
        }
        return {};
    }
private:
    std::optional<std::vector<int>> get_digits(int pad) {
        if(auto line = get_line()) {
            const std::regex expr("([0-9])");
            std::vector<int> result(1, pad); // left pad

            std::transform(
                std::sregex_token_iterator{line->begin(), line->end(), expr},
                std::sregex_token_iterator{},
                std::back_inserter(result),
                [](std::string const &s){ return std::stoi(s); });

            result.push_back(pad); // right pad

            return result;
        }
        return {};
    }
};

void part1(int pad) {
    Data data("data09.txt");
    auto height = data.get_grid(pad);
    int total_risk = 0;

    for(int y = 1; y < height.size() - 1; ++y) {
        for(int x = 1; x < height[y].size() - 1; ++x) {
            if( height[y][x] < height[y-1][x] &&
                height[y][x] < height[y+1][x] &&
                height[y][x] < height[y][x-1] &&
                height[y][x] < height[y][x+1])
            {
                total_risk += 1 + height[y][x];
            }
        }
    }
    std::cout << "total risk: " << total_risk << "\n";
}

int calculate_basin_size(PaddedGrid<bool> &visited, const PaddedGrid<int> &height, int x, int y, int max_height) {
    if(height[y][x] == max_height || visited[y][x]) return 0;
    visited[y][x] = true;
    int total = 1;
    total += calculate_basin_size(visited, height, x - 1, y, max_height);
    total += calculate_basin_size(visited, height, x + 1, y, max_height);
    total += calculate_basin_size(visited, height, x, y - 1, max_height);
    total += calculate_basin_size(visited, height, x, y + 1, max_height);
    return total;
}

void part2(int max_height) {
    Data data("data09.txt");
    auto height = data.get_grid(max_height);
    std::vector<std::pair<int, int>> low_points;

    for(int y = 1; y < height.size() - 1; ++y) {
        for(int x = 1; x < height[y].size() - 1; ++x) {
            if( height[y][x] < height[y-1][x] &&
                height[y][x] < height[y+1][x] &&
                height[y][x] < height[y][x-1] &&
                height[y][x] < height[y][x+1])
            {
                low_points.push_back({x, y});
            }
        }
    }
    std::vector<int> basin_size;
    PaddedGrid<bool> visited(height.size(), std::vector<bool>(height[0].size(), false));

    for(const auto &[x, y] : low_points) {
        basin_size.push_back(calculate_basin_size(visited, height, x, y, max_height));
    }
    std::sort(basin_size.begin(), basin_size.end());

    int product_of_sizes = 1;
    for(auto it = basin_size.rbegin(); it != basin_size.rbegin() + 3; ++it) {
        product_of_sizes *= *it;
    }
    std::cout << "Product of 3 largest basin sizes: " << product_of_sizes << "\n";
}

int main() {
    const int max_height = 9;

    part1(max_height);

    part2(max_height);

    return 0;
}
