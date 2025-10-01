#include "state.h"

#include <algorithm>

State::State(const std::string &state_str, const char white_char, const char black_char, const char empty_char) {
    if (state_str.length() != 65) {
        throw std::invalid_argument("State string must be 65 characters long");
    }

    current_player = state_str[0] == 'W' ? utils::Piece::White : utils::Piece::Black;

    for (std::uint8_t row = 0; row < utils::SIZE; ++row) {
        for (std::uint8_t col = 0; col < utils::SIZE; ++col) {
            const char c = state_str[1 + row * utils::SIZE + col];
            utils::Piece piece;
            if (c == white_char) {
                piece = utils::Piece::White;
            } else if (c == black_char) {
                piece = utils::Piece::Black;
            } else if (c == empty_char) {
                piece = utils::Piece::Empty;
            } else {
                throw std::invalid_argument("Invalid character in state string: " + std::string(1, c));
            }

            board.set(row, col, piece);
        }
    }
}

std::unique_ptr<State> State::make_move(const utils::Move move) const {
    auto new_state = std::make_unique<State>(*this);
    new_state->make_move_on_state(move);
    return new_state;
}

void State::make_move_on_state(const utils::Move move) {
    if (move.is_pass) {
        current_player = opponent(current_player);
        return;
    }

    for (auto d: utils::DIRECTIONS) {
        auto captures = get_captures_in_direction(board, current_player, move.row, move.col, d);
        for (const auto &cap: captures) {
            board.set(cap.row, cap.col, current_player);
        }
    }

    board.set(move.row, move.col, current_player);
    current_player = opponent(current_player);
}


std::vector<utils::Move> get_legal_moves(const BitBoard &board, const utils::Piece player) {
    std::vector<utils::Move> legal_moves;
    for (std::uint8_t row = 0; row < utils::SIZE; ++row) {
        for (std::uint8_t col = 0; col < utils::SIZE; ++col) {
            if (is_legal_move(board, player, row, col)) {
                legal_moves.emplace_back(row, col);
            }
        }
    }

    return legal_moves;
}

bool is_legal_move(const BitBoard &board, const utils::Piece player, const std::uint8_t row,
                   const std::uint8_t col) {
    if (board.get(row, col) != utils::Piece::Empty) {
        return false;
    }

    return std::ranges::any_of(utils::DIRECTIONS, [&](auto d) {
        return !get_captures_in_direction(board, player, row, col, d).empty();
    });
}

std::vector<utils::Move> get_captures_in_direction(const BitBoard &board, const utils::Piece player,
                                                   const std::uint8_t row,
                                                   const std::uint8_t col,
                                                   const utils::Direction &dir) {
    auto r = row + dir.dy;
    auto c = col + dir.dx;

    if (is_on_board(r, c) && // Check bounds
        board.get(r, c) != opponent(player)) {
        return {};
    }

    std::vector<utils::Move> captures;
    while (is_on_board(r, c)) {
        // While in bounds
        if (board.get(r, c) == utils::Piece::Empty) {
            return {};
        }
        if (board.get(r, c) == player) {
            return captures;
        }

        captures.emplace_back(r, c);
        r += dir.dy;
        c += dir.dx;
    }

    captures.clear();
    return captures;
}
