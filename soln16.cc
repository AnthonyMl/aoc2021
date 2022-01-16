#include "aoc.hpp"
#include <iostream>
#include <optional>

// part 1: 984
// part 2: 1015320896946

class Data: public BaseData {
    uint8_t hex_to_binary(char c) {
        if(c >= '0' && c <= '9') return c - '0';

        return c - 'A' + 10;
    }

public:
    Data(const char *filename): BaseData(filename) {}

    std::optional<uint8_t> get_hex_digit() {
        char c;
        stream >> c;
        if(stream.eof()) return {};
        return hex_to_binary(c);
    }
};

class BitStream {
private:
    Data data;
    uint8_t bits;
    int index {-1}; // 0-3 index into the hex digit lsb-msb
    bool eof {false};
    unsigned long long read_count {0};

    void get_next_hex() {
        auto b = data.get_hex_digit();
        if(!b) eof = true;
        else bits = *b;
        index = 3;
    }
public:
    BitStream(const char *filename): data(filename) { }

    uint8_t get_bit() {
        if(index < 0) get_next_hex();
        uint8_t result = (bits & (1 << index)) >> index;
        index--;
        read_count++;
        return result;
    }

    uint32_t get_bits(int bit_count) {
        if(bit_count > 32) throw std::out_of_range("bit_count > 32");

        uint32_t result = 0;
        for(;bit_count > 0; --bit_count) {
            result |= get_bit() << (bit_count - 1);
        }
        return result;
    }

    void flush() {
        get_next_hex();
        // assumes version_id != 0
        while(bits == 0 && !eof) get_next_hex();
    }

    bool is_open() const {
        return !eof;
    }

    unsigned long long bits_read() const {
        return read_count;
    }
};

enum Type {
    Sum = 0,
    Product = 1,
    Minimum = 2,
    Maximum = 3,
    Literal = 4,
    Greater = 5,
    Less    = 6,
    Equal   = 7,
};

using IntType = uint64_t;

struct PResult {
    int version_sum {0};
    IntType value;
};

class Operation {
private:
    std::vector<IntType> values;
public:
    void insert_argument(IntType arg) {
        values.push_back(arg);
    }

    IntType execute(int type_id) {
        switch(type_id) {
        case Type::Sum: {
                IntType result = 0;
                for(const auto &x : values) result += x;
                return result;
            }
        case Type::Product: {
                IntType result = 1;
                for(const auto &x : values) result *= x;
                return result;
            }
        case Type::Minimum: {
                IntType result = values[0];
                for(const auto &x : values) result = std::min(result, x);
                return result;
            }
        case Type::Maximum: {
                IntType result = values[0];
                for(const auto &x : values) result = std::max(result, x);
                return result;
            }
        case Type::Greater:
            return values[0] > values[1];
        case Type::Less:
            return values[0] < values[1];
        case Type::Equal:
            return values[0] == values[1];
        }
        throw std::invalid_argument("invalid type id");
    }
};

PResult parse_packet(BitStream &);

IntType parse_literal(BitStream &data) {
    constexpr int chunk_size = 5;
    IntType result = 0;

    while(true) {
        auto bits = data.get_bits(chunk_size);

        result |= bits & 0b1111;

        if(bits & (1 << (chunk_size - 1))) result <<= (chunk_size - 1);
        else break;
    }
    return result;
}

PResult parse_operator(int type_id, BitStream &data) {
    PResult result;
    Operation op;
    auto length_type = data.get_bit();
    if(length_type) {
        for(auto packet_count = data.get_bits(11); packet_count > 0; --packet_count) {
            auto pr = parse_packet(data);
            result.version_sum += pr.version_sum;
            op.insert_argument(pr.value);
        }
    } else {
        auto subpackets_size = data.get_bits(15);
        auto packet_start = data.bits_read();
        while(data.bits_read() < packet_start + subpackets_size) {
            auto pr = parse_packet(data);
            result.version_sum += pr.version_sum;
            op.insert_argument(pr.value);
        }
    }
    result.value = op.execute(type_id);
    return result;
}

PResult parse_packet(BitStream &data) {
    auto version = data.get_bits(3);
    auto type_id = data.get_bits(3);

    PResult result;
    result.version_sum += version;

    switch(type_id) {
    case Type::Literal:
        result.value = parse_literal(data);
        break;
    default: {
            auto pr = parse_operator(type_id, data);
            result.version_sum += pr.version_sum;
            result.value = pr.value;
            break;
        }
    }
    return result;
}

void part1() {
    BitStream data("data16.txt");

    int version_sum = 0;
    while(data.is_open()) {
        version_sum += parse_packet(data).version_sum;

        data.flush();
    }
    std::cout << "version sum: " << version_sum << "\n";
}

void part2() {
    BitStream data("data16.txt");

    IntType value = 0;
    while(data.is_open()) {
        value = parse_packet(data).value;

        data.flush();
    }
    std::cout << "value: " << value << "\n";
}

int main() {
    part1();

    part2();

    return 0;
}
