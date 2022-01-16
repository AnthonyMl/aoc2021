#include "aoc.hpp"
#include <iostream>
#include <vector>

// part 1: 513
// part 2:

struct Grid {
    Grid() {}

    Grid(const Grid &g) {
        for(const auto &row : g.m) {
            m.push_back(std::vector<char>(row.size(), '.'));
        }
    }

    static Grid parse_input() {
        BaseData data("data25f.txt");
        Grid g;
        while(const auto &line = data.get_line()) {
            g.m.push_back(std::vector<char>(line->begin(), line->end()));
        }
        return g;
    }

    bool operator==(const Grid &g) {
        for(int row = 0; row < m.size(); ++row) {
            for(int col = 0; col < m[row].size(); ++col) {
                if(m[row][col] != g.m[row][col]) return false;
            }
        }
        return true;
    }

    Grid step() const {
        return step_east().step_south();
    }

    void print() const {
        for(const auto &row : m) {
            std::cout << std::string(row.begin(), row.end()) << "\n";
        }
    }
private:
    std::vector<std::vector<char>> m;

    Grid step_east() const {
        Grid g(*this);
        for(int row = 0; row < m.size(); ++row) {
            for(int col = 0; col < m[row].size() - 1; ++col) {
                if(m[row][col] == '>') {
                    if(m[row][col+1] == '.') g.m[row][col+1] = '>';
                    else                     g.m[row][col] = '>';
                }
                if(m[row][col] == 'v') g.m[row][col] = 'v';
            }
            if(m[row].back() == '>') {
                if(m[row][0] == '.') g.m[row][0] = '>';
                else                 g.m[row].back() = '>';
            }
            if(m[row].back() == 'v') g.m[row].back() = 'v';
        }
        return g;
    }

    Grid step_south() const {
        Grid g(*this);
        for(int row = 0; row < m.size() - 1; ++row) {
            for(int col = 0; col < m[row].size(); ++col) {
                if(m[row][col] == 'v') {
                    if(m[row+1][col] == '.') g.m[row+1][col] = 'v';
                    else                     g.m[row][col] = 'v';
                }
                if(m[row][col] == '>') g.m[row][col] = '>';
            }
        }
        for(int col = 0; col < m[0].size(); ++col) {
            if(m.back()[col] == 'v') {
                if(m[0][col] == '.') g.m[0][col] = 'v';
                else                 g.m.back()[col] = 'v';
            }
            if(m.back()[col] == '>') g.m.back()[col] = '>';
        }
        return g;
    }
};

void part1() {
    auto g = Grid::parse_input();

    int i = 1;
    for(;; ++i) {
        auto g_new = g.step();

        if(g == g_new) break;

        g = g_new;
    }
    std::cout << "stopped moving after: " << i << " steps\n";
}

int main() {
    part1();

    return 0;
}
