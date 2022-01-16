#include "aoc.hpp"
#include <iostream>
#include <vector>
#include <optional>
#include <numeric>

// part 1: 1766
// part 2: 1797

std::optional<int> get_int(BaseData &data) {
    if(auto line = data.get_line()) {
        if(line->length() > 1) return std::stoi(*line);
    }
    return {};
}

class SlidingWindow {
private:
    static const int size = 3;
    std::vector<int> values;
    int index = 0;
    int sum = 0;
public:
    SlidingWindow(BaseData &data) {
        for(int i = 0; i < size; ++i) {
            if(auto v = get_int(data)) {
                sum += *v;
                values.push_back(*v);
            } else {
                throw std::invalid_argument("Error reading input data: insufficient inputs for window");
            }
        }
    }

    int value() {
        return std::accumulate(values.begin(), values.end(), 0);
    }

    std::optional<int> next(BaseData &data) {
        if(auto v = get_int(data)) {
            values[index] = *v;
            index = (index + 1) % size;
            return value();
        }
        return {};
    }
};

void part1(BaseData &&data) {
    int num_larger = 0;
    int last_input = std::numeric_limits<int>::max();
    while(auto next = get_int(data)) {
        if(next > last_input) num_larger++;

        last_input = *next;
    }
    std::cout << "larger than previous: " << num_larger << "\n";
}

void part2(BaseData &&data) {
    SlidingWindow window(data);

    int num_larger = 0;
    int last_input = window.value();
    while(auto next = window.next(data)) {
        if(next > last_input) num_larger++;

        last_input = *next;
    }
    std::cout << "larger than previous: " << num_larger << "\n";
}

int main() {
    auto filename = "data01.txt";

    part1({filename});

    part2({filename});

    return 0;
}

