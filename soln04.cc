#include "aoc.hpp"
#include <iostream>
#include <algorithm>
#include <regex>
#include <vector>
#include <array>

// part 1: 2496
// part 2: 25925

class Board {
public:
    static const int size = 5;

    Board(const int input_numbers[size*size]) {
        for(int i = 0; i < size * size; ++i) numbers[i] = input_numbers[i];
    }

    void mark(int value) {
        for(int i = 0; i < size * size; ++i) {
            if(numbers[i] == value) {
                marked[i] = true;
                return;
            }
        }
    }

    bool has_won() {
        bool winning;
        for(int y = 0; y < size; ++y) {
            for(int x = 0; x < size; ++x) {
                if(!marked[y * size + x]) {
                    winning = false;
                    break;
                }
                winning = true;
            }
            if(winning) return true;
        }

        for(int x = 0; x < size; ++x) {
            for(int y = 0; y < size; ++y) {
                if(!marked[y * size + x]) {
                    winning = false;
                    break;
                }
                winning = true;
            }
            if(winning) return true;
        }

        for(int i = 0; i < size; ++i) {
            if(!marked[i * size + i]) return false;
        }
        return true;
    }

    int unmarked_value() {
        int sum = 0;
        for(int i = 0; i < size * size; ++i) if(!marked[i]) sum += numbers[i];
        return sum;
    }

private:
    std::array<int, size * size> numbers;
    std::array<bool, size * size> marked = {};
};

void parse_line_into_numbers(std::vector<int> &result, const std::string &&line) {
    const std::regex expr("([0-9]+)");

    std::transform(
        std::sregex_token_iterator{line.begin(), line.end(), expr},
        std::sregex_token_iterator{},
        std::back_inserter(result),
        [](std::string const &s){ return std::stoi(s); });
}

std::vector<Board> construct_boards(BaseData &data) {
    std::vector<Board> boards;
    int row_index = 0;
    std::vector<int> values;
    while(auto line = data.get_line()) {
        if(line->size() == 0) continue;
        parse_line_into_numbers(values, std::move(*line));

        row_index++;
        if(row_index % Board::size == 0) {
            boards.push_back(Board(values.data()));
            values.clear();
        }
    }
    return boards;
}

void part1() {
    BaseData data("data04.txt");
    std::vector<int> called_numbers;
    parse_line_into_numbers(called_numbers, *data.get_line());
    auto boards = construct_boards(data);

    for(int number : called_numbers) {
        for(auto &board : boards) {
            board.mark(number);

            if(board.has_won()) {
                std::cout << "winner: " << number * board.unmarked_value() << "\n";
                return;
            }
        }
    }
}

void part2() {
    BaseData data("data04.txt");
    std::vector<int> called_numbers;
    parse_line_into_numbers(called_numbers, *data.get_line());
    auto boards = construct_boards(data);
    Board last_winner = boards[0];
    int last_winning_number;

    for(int number : called_numbers) {
        for(int i = 0; i < boards.size(); ++i) {
            boards[i].mark(number);

            if(boards[i].has_won()) {
                last_winner = boards[i];
                last_winning_number = number;
                if(boards.size() > 1) {
                    std::swap(boards[i], boards.back());
                    boards.pop_back();
                    i--;
                } else if(boards.size() == 1) {
                    std::cout << "Last winner: " <<
                        last_winning_number * last_winner.unmarked_value() << "\n";
                    return;
                }
            }
        }
    }
}

int main() {
    part1();

    part2();

    return 0;
}
