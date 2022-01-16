#include "aoc.hpp"
#include <iostream>
#include <unordered_map>

// part 1: 415
// part 2: 2864

template <typename T>
class Grid {
public:
    Grid(){}
    Grid(int width, int height): d(height, std::vector<T>(width, 0)) { }
    Grid(int width, int height, T v): d(height, std::vector<T>(width, v)) { }

    static Grid<int> parse_data() {
        BaseData data("data15.txt");
        Grid<int> g;
        while(const auto &line = data.get_line()) g.d.push_back(line_to_digits(*line));
        return g;
    }

    int height() const { return d.size(); }
    int width() const { return d[0].size(); }

    T at(int x, int y) const { return d[y][x]; }
    T at(Point2 p) const { return d[p.y][p.x]; }

    void set(int x, int y, const T &v) { d[y][x] = v; }
    void set(Point2 p, const T &v) { d[p.y][p.x] = v; }

    void print() const {
        for(const auto &row : d) {
            for(const auto &entry : row) std::cout << entry << " ";
            std::cout << "\n";
        }
    }
private:
    std::vector<std::vector<T>> d;
};


template <typename T>
std::vector<Point2> neighbors(const Grid<T> &g, Point2 p) {
    std::vector<Point2> result;
    if(p.x > 0) result.push_back({p.x-1, p.y});
    if(p.y > 0) result.push_back({p.x, p.y-1});
    if(p.x < g.width() - 1) result.push_back({p.x+1, p.y});
    if(p.y < g.height() - 1) result.push_back({p.x, p.y+1});
    return result;
}

int risk(const Grid<int> &g) {
    std::unordered_map<Point2, int, point2_hash> todo{{Point2{g.width() - 1,g.height() - 1}, 0}};
    Grid<int> costs(g.width(), g.height(), std::numeric_limits<int>::max());

    while(!todo.empty()) {
        auto [p, cost] = *todo.begin();
        for(const auto &n : neighbors(g, p)) {
            if(cost + g.at(n) < costs.at(n)) {
                todo[n] = cost + g.at(n);
                costs.set(n, cost + g.at(n));
            }
        }
        todo.erase(p);
    }
    return costs.at(0, 0) + g.at(g.width() - 1, g.height() - 1) - g.at(0, 0);
}

Grid<int> extend(const Grid<int> &g) {
    const int d = 5;
    Grid r(g.width() * d, g.height() * d, 0);

    for(int y = 0; y < g.height(); ++y) {
        for(int x = 0; x < g.width(); ++x) {
            for(int yi = 0; yi < d; ++yi) {
                for(int xi = 0; xi < d; ++xi) {
                    r.set(
                        g.width() * xi + x,
                        g.height() * yi + y,
                        (g.at(x, y) + xi + yi - 1) % 9 + 1);
                }
            }
        }
    }
    return r;
}

void part1() {
    auto grid = Grid<int>::parse_data();

    std::cout << "total cost: " << risk(grid) << "\n";
}

void part2() {
    auto grid = extend(Grid<int>::parse_data());

    std::cout << "total cost: " << risk(grid) << "\n";
}

int main() {
    part1();

    part2(); // slow

    return 0;
}
