#include "aoc.hpp"
#include <vector>
#include <optional>
#include <regex>
#include <iostream>

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
        if(const auto &line = get_line()) {
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

void step_octopus(PaddedGrid<int> &energy, PaddedGrid<bool> &flashed, int x, int y, int flash_energy) {
    if(flashed[y][x] || y == 0 || x == 0 || y == energy.size() - 1 || x == energy[0].size() - 1) return;

    energy[y][x]++;

    if(energy[y][x] < flash_energy) return;

    flashed[y][x] = true;
    step_octopus(energy, flashed, x-1, y-1, flash_energy);
    step_octopus(energy, flashed, x-1, y  , flash_energy);
    step_octopus(energy, flashed, x-1, y+1, flash_energy);
    step_octopus(energy, flashed, x  , y-1, flash_energy);
    step_octopus(energy, flashed, x  , y+1, flash_energy);
    step_octopus(energy, flashed, x+1, y-1, flash_energy);
    step_octopus(energy, flashed, x+1, y  , flash_energy);
    step_octopus(energy, flashed, x+1, y+1, flash_energy);
}

void part1() {
    Data data("data11.txt");
    const int max_steps = 100;
    const int flash_energy = 10;
    auto energy = data.get_grid(0);
    unsigned int flash_count = 0;

    for(int i = 0; i < max_steps; ++i) {
        // allocate outside and zero here instead?
        PaddedGrid<bool> flashed(energy.size(), std::vector<bool>(energy[0].size(), false));

        for(int y = 1; y < energy.size() - 1; ++y) {
            for(int x = 1; x < energy[0].size() - 1; ++x) {
                step_octopus(energy, flashed, x, y, flash_energy);
            }
        }
        for(int y = 1; y < energy.size() - 1; ++y) {
            for(int x = 1; x < energy[0].size() - 1; ++x) {
                if(flashed[y][x]) {
                    energy[y][x] = 0;
                    flash_count++;
                }
            }
        }
    }
    std::cout << "flash count: " << flash_count << "\n";
}

void part2() {
    Data data("data11.txt");
    const int flash_energy = 10;
    auto energy = data.get_grid(0);

    for(int i = 0;; ++i) {
        PaddedGrid<bool> flashed(energy.size(), std::vector<bool>(energy[0].size(), false));
        unsigned int flash_count = 0;

        for(int y = 1; y < energy.size() - 1; ++y) {
            for(int x = 1; x < energy[0].size() - 1; ++x) {
                step_octopus(energy, flashed, x, y, flash_energy);
            }
        }
        for(int y = 1; y < energy.size() - 1; ++y) {
            for(int x = 1; x < energy[0].size() - 1; ++x) {
                if(flashed[y][x]) {
                    energy[y][x] = 0;
                    flash_count++;
                }
            }
        }
        if(flash_count == (energy.size() - 2) * (energy[0].size() - 2)) {
            std::cout << "synchronous flash at step: " << i + 1 << "\n";
            break;
        }
    }
}

int main() {
    part1();

    part2();

    return 0;
}
