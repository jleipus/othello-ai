#include "bitboard.h"

BitBoard::BitBoard() : black_bitboard(0), white_bitboard(0) {}

utils::Piece BitBoard::get(const std::uint8_t row, const std::uint8_t col) const {
    const auto bit_position = 1ULL << (row * 8 + col);
    if (white_bitboard & bit_position) {
        return utils::Piece::White;
    }
    if (black_bitboard & bit_position) {
        return utils::Piece::Black;
    }

    return utils::Piece::Empty;
}

void BitBoard::set(const std::uint8_t row, const std::uint8_t col, const utils::Piece value) {
    const auto bit_position = 1ULL << (row * 8 + col);
    if (value == utils::Piece::White) {
        white_bitboard |= bit_position;
        black_bitboard &= ~bit_position;
    } else if (value == utils::Piece::Black) {
        black_bitboard |= bit_position;
        white_bitboard &= ~bit_position;
    } else {
        white_bitboard &= ~bit_position;
        black_bitboard &= ~bit_position;
    }
}

std::string BitBoard::to_string() const {
    std::string result;
    for (std::uint64_t row = 0; row < utils::SIZE; ++row) {
        for (std::uint64_t col = 0; col < utils::SIZE; ++col) {
            if (const auto piece = get(row, col); piece == utils::Piece::White) {
                result += 'W';
            } else if (piece == utils::Piece::Black) {
                result += 'B';
            } else {
                result += ' ';
            }

            if (col < utils::SIZE - 1) {
                result += " | ";
            }
        }

        result += '\n';
        if (row < utils::SIZE - 1) {
            result += "-----------------------------\n";
        }
    }
    return result;
}
