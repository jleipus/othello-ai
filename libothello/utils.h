//
// Created by joris on 9/28/25.
//

#ifndef OTHELLO_AI_BOARD_H
#define OTHELLO_AI_BOARD_H

#include <array>
#include <format>
#include <string>
#include <cstdint>
#include <map>

namespace utils {
    constexpr std::uint8_t SIZE = 8;

    enum class Piece : uint8_t { Empty = 0, Black = 1, White = 2 };

    inline Piece opponent(const Piece piece) {
        if (piece == Piece::Black) {
            return Piece::White;
        }
        if (piece == Piece::White) {
            return Piece::Black;
        }
        return Piece::Empty;
    }

    class Move {
    public:
        int row;
        int col;
        bool is_pass;

        Move() : row(-1), col(-1), is_pass(false) {}

        Move(const int row, const int col,
             const bool is_pass = false)
            : row(row), col(col), is_pass(is_pass) {}

        [[nodiscard]] bool is_empty() const {
            return (row == -1 && col == -1 && !is_pass);
        }

        [[nodiscard]] std::string to_string() const {
            if (is_empty()) {
                return "empty";
            }

            if (is_pass) {
                return "pass";
            }
            auto row_char = static_cast<char>('1' + row);
            auto col_char = static_cast<char>('1' + col);
            return std::format("({},{})", row_char, col_char);
        }
    };

    struct Direction {
        int dx;
        int dy;

        // Converts to a shift value for bitboard operations
        [[nodiscard]] constexpr int to_shift() const { return dy * SIZE + dx; }
    };

    inline constexpr std::array<Direction, 8> DIRECTIONS{
        {
            {-1, -1}, // NW
            {0, -1}, // N
            {1, -1}, // NE
            {-1, 0}, // W
            {1, 0}, // E
            {-1, 1}, // SW
            {0, 1}, // S
            {1, 1} // SE
        }
    };

    inline constexpr std::array<std::uint8_t, 4> CORNERS{
        0, 7, 56, 63
    };

    // Squares adjacent to corners
    inline const std::map<std::uint8_t, std::array<std::uint8_t, 2> > C_SQUARES{
        {{0, {1, 8}}, {7, {6, 15}}, {56, {48, 57}}, {63, {55, 62}}}
    };

    // Squares diagonally adjacent to corners
    inline const std::map<std::uint8_t, std::uint8_t> X_SQUARES{
        {{0, 9}, {7, 14}, {56, 49}, {63, 54}}
    };

    inline constexpr std::array<std::uint8_t, 24> EDGES{
        1, 2, 3, 4, 5, 6, // Top edge
        8, 16, 24, 32, 40, 48, // Left edge
        15, 23, 31, 39, 47, 55, // Right edge
        57, 58, 59, 60, 61, 62 // Bottom edge
    };
} // namespace utils

#endif  // OTHELLO_AI_BOARD_H
