//
// Created by joris on 9/28/25.
//

#ifndef OTHELLO_AI_BITBOARD_H
#define OTHELLO_AI_BITBOARD_H

#include <cstdint>

#include "utils.h"

class BitBoard {
public:
    BitBoard();

    BitBoard(const BitBoard &other) = default;

    [[nodiscard]] utils::Piece get(std::uint8_t row, std::uint8_t col) const;

    void set(std::uint8_t row, std::uint8_t col, utils::Piece value);

    [[nodiscard]] std::uint64_t get_black_bitboard() const {
        return black_bitboard;
    }

    [[nodiscard]] std::uint64_t get_white_bitboard() const {
        return white_bitboard;
    }

    [[nodiscard]] std::string to_string() const;

private:
    uint64_t black_bitboard;
    uint64_t white_bitboard;
};

#endif  // OTHELLO_AI_BITBOARD_H
