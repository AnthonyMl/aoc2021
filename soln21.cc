#include "aoc.hpp"
#include <array>
#include <iostream>
#include <regex>
#include <unordered_map>

// part 1: 903630
// part 2: 303121579983974

class Die {
private:
    uint64_t roll_count {0};
    int sides;
public:
    Die(int sides): sides(sides) {}

    int roll() {
        auto r = 1 + (roll_count % sides);
        roll_count++;
        return r;
    }

    uint64_t total_rolls() {
        return roll_count;
    }
};

std::pair<int, int> parse_positions() {
    BaseData data("data21.txt");

    auto line = data.get_line();
    const std::regex expr1{"Player 1 starting position: ([0-9]+)"};
    std::smatch match1;
    std::regex_match(*line, match1, expr1);

    line = data.get_line();
    const std::regex expr2{"Player 2 starting position: ([0-9]+)"};
    std::smatch match2;
    std::regex_match(*line, match2, expr2);

    return { std::stoi(match1[1]), std::stoi(match2[1]) };
}

void part1() {
    const int score_limit = 1000;
    std::array<int, 2> score{0, 0};
    std::array<int, 2> position;

    auto [p0, p1] = parse_positions();
    position[0] = p0;
    position[1] = p1;

    Die die(100);
    int turn = 0;
    while(score[0] < score_limit && score[1] < score_limit) {
        int roll = die.roll() + die.roll() + die.roll();

        position[turn] = 1 + ((position[turn] - 1 + roll) % 10);

        score[turn] += position[turn];
        turn = (turn + 1) % 2;
    }
    std::cout << "result: " << score[turn] * die.total_rolls() << "\n";
}


using IntType = uint64_t;
using Dirac = std::array<IntType, 7>;

struct Board {
    std::array<uint8_t, 2> score {0, 0};
    std::array<uint8_t, 2> position;
    uint8_t turn {0};
    IntType instance_count {1};
};

constexpr Dirac dirac() {
    Dirac result{0};
    for(int r1 = 1; r1 < 4; ++r1) {
        for(int r2 = 1; r2 < 4; ++r2) {
            for(int r3 = 1; r3 < 4; ++r3) {
                result[r1 + r2 + r3 - 3]++;
            }
        }
    }
    return result;
}

void part2() {
    const int score_limit = 21;
    constexpr Dirac rolls{dirac()};
    std::array<IntType, 2> wins{0, 0};
    Board b;
    auto [p0, p1] = parse_positions();
    b.position[0] = p0;
    b.position[1] = p1;
    std::vector<Board> boards{b};

    while(!boards.empty()) {
        auto board = boards.back();
        boards.pop_back();

        if(board.score[(board.turn + 1) % 2] >= score_limit) {
            wins[board.turn] += board.instance_count;
            continue;
        }

        for(int roll = 3; roll <= 9; ++roll) {
            auto count = rolls[roll - 3];
            Board b = board;
            b.position[b.turn] = 1 + ((b.position[b.turn] - 1 + roll) % 10);
            b.instance_count = board.instance_count * count;

            b.score[b.turn] += b.position[b.turn];
            b.turn = (b.turn + 1) % 2;
            boards.push_back(b);
        }
    }
    std::cout << "Winner wins: " << std::max(wins[0], wins[1]) << "\n";
}

int main() {
    part1();

    part2();

    return 0;
}
