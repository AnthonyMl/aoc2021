#include "aoc.hpp"
#include <iostream>
#include <optional>
#include <regex>
#include <string>

// part 1: 1383564
// part 2: 1488311643

enum class Direction {
    Forward,
    Down,
    Up,
};

class Data: public BaseData {
public:
    struct Command {
        Direction direction;
        unsigned int amount;
    };

    Data(const char *filename): BaseData(filename) { }

    std::optional<Command> get_command() {
        if(auto line = get_line()) return parse_command(*line);
        return {};
    }
private:
    static std::optional<Command> parse_command(const std::string &data) {
        const std::string integer_match(" ([0-9]+)");
        const std::regex expr_forward("forward" + integer_match);
        const std::regex expr_down("down" + integer_match);
        const std::regex expr_up("up" + integer_match);
        std::smatch match;
        Command command;
        if(std::regex_match(data, match, expr_forward)) {
            command.direction = Direction::Forward;
        } else if(std::regex_match(data, match, expr_down)) {
            command.direction = Direction::Down;
        } else if(std::regex_match(data, match, expr_up)) {
            command.direction = Direction::Up;
        } else {
            return {};
        }
        if(match.size() == 2) {
            command.amount = std::stoi(match[1].str());
            return command;
        }
        return {};
    }
};

void part1(Data &&data) {
    unsigned int distance = 0;
    unsigned int depth = 0;

    while(auto command = data.get_command()) {
        switch(command->direction) {
            case Direction::Forward:
                distance += command->amount;
                break;
            case Direction::Down:
                depth += command->amount;
                break;
            case Direction::Up:
                depth -= command->amount;
                break;
        }
    }
    std::cout << "distance * depth: " << distance * depth << "\n";
}

void part2(Data &&data) {
    unsigned int distance = 0;
    unsigned int depth = 0;
    unsigned int aim = 0;

    while(auto command = data.get_command()) {
        switch(command->direction) {
            case Direction::Forward:
                distance += command->amount;
                depth += aim * command->amount;
                break;
            case Direction::Down:
                aim += command->amount;
                break;
            case Direction::Up:
                aim -= command->amount;
                break;
        }
    }
    std::cout << "distance * depth: " << distance * depth << "\n";
}

int main() {
    auto filename = "data02.txt";

    part1({filename});

    part2({filename});

    return 0;
}
