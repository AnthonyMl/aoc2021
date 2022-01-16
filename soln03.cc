#include "aoc.hpp"
#include <string>
#include <optional>
#include <iostream>
#include <vector>

// part 1: 3969000
// part 2: 4267809

class Data: public BaseData {
private:
    int number_size_;
public:
    Data(const char *filename): BaseData(filename) {
        number_size_ = get_line()->length();
        stream.clear();
        stream.seekg(0, stream.beg);
    }

    int number_size() {
        return number_size_;
    }
};

void part1() {
    Data data("data03.txt");
    std::vector<unsigned int> one_count(data.number_size(), 0);
    int num_numbers = 0;

    while(const auto &bits = data.get_line()) {
        if(bits->length() == 0) break;

        for(int i = 0; i < bits->length(); ++i) {
            if(bits.value()[i] == '1') one_count[i]++;
        }
        num_numbers++;
    }
    int rates[]{0, 0}; // gamma, epsilon

    for(int i = data.number_size() - 1; i >= 0; i--) {
        int rate_index = static_cast<int>(one_count[i] >= num_numbers / 2);

        rates[rate_index] |= 1 << (data.number_size() - i - 1);
    }
    std::cout << "power consumption: " << rates[0] * rates[1] << "\n";
}

std::pair<std::vector<std::string>, std::vector<std::string>>
split_by_major_bit(const std::vector<std::string> &ns, int bit_index) {
    std::vector<std::string> indexed_one;
    std::vector<std::string> indexed_zero;

    for(const auto &number : ns) {
        if(number[bit_index] == '1') {
            indexed_one.push_back(number);
        } else {
            indexed_zero.push_back(number);
        }
    }
    return { indexed_one, indexed_zero };
}

int binary_string_to_int(const std::string &s) {
    int number = 0;
    for(int i = 0; i < s.size(); ++i) {
        if(s[i] == '1') number |= 1 << (s.size() - i - 1);
    }
    return number;
}

struct ParseResult {
    std::vector<std::string> numbers_oxygen;
    std::vector<std::string> numbers_co2;
    int number_size;
};

ParseResult parse_data() {
    ParseResult result;
    Data data("data03.txt");
    int num_leading_ones = 0;
    int bit_count = 0;

    result.number_size = data.number_size();

    while(const auto &bit = data.get_line()) {
        if(bit->length() == 0) break;

        if(bit.value()[0] == '1') {
            num_leading_ones++;
            result.numbers_oxygen.push_back(*bit);
        } else {
            result.numbers_co2.push_back(*bit);
        }
        bit_count++;
    }
    if(num_leading_ones < bit_count / 2) {
        std::swap(result.numbers_oxygen, result.numbers_co2);
    }
    return result;
}

void part2() {
    auto [numbers_oxygen, numbers_co2, number_size] = parse_data();

    for(int i = 1; i < number_size; ++i) {
        auto [one, zero] = split_by_major_bit(numbers_oxygen, i);

        if(one.size() >= zero.size()) {
            std::swap(numbers_oxygen, one);
        } else {
            std::swap(numbers_oxygen, zero);
        }
        if(numbers_oxygen.size() == 1) break;
    }

    for(int i = 1; i < number_size; ++i) {
        auto [one, zero] = split_by_major_bit(numbers_co2, i);

        if(one.size() >= zero.size()) {
            std::swap(numbers_co2, zero);
        } else {
            std::swap(numbers_co2, one);
        }
        if(numbers_co2.size() == 1) break;
    }

    std::cout << "life support rating: " <<
        binary_string_to_int(numbers_oxygen[0]) *
        binary_string_to_int(numbers_co2[0]) << "\n";
}

int main() {
    part1();

    part2();

    return 0;
}
