//
// Created by joris on 9/28/25.
//

#ifndef OTHELLO_AI_STATE_H
#define OTHELLO_AI_STATE_H

#include <memory>
#include <vector>

#include "bitboard.h"
#include "utils.h"

class State {
public:
    State() : current_player(utils::Piece::White) {}

    explicit State(const std::string &state_str, char white_char = 'W', char black_char = 'B', char empty_char = 'E');

    State(const State &other) = default;

    [[nodiscard]] std::unique_ptr<State> make_move(utils::Move move) const;

    void make_move_on_state(utils::Move move);

    [[nodiscard]] utils::Piece get_current_player() const {
        return current_player;
    }

    [[nodiscard]] BitBoard get_board() const {
        return board;
    }

private:
    utils::Piece current_player;
    BitBoard board;
};

[[nodiscard]] std::vector<utils::Move> get_legal_moves(const BitBoard &board, utils::Piece player);

[[nodiscard]] bool is_legal_move(const BitBoard &board, utils::Piece player, std::uint8_t row,
                                 std::uint8_t col);

[[nodiscard]] std::vector<utils::Move> get_captures_in_direction(const BitBoard &board, utils::Piece player,
                                                                 std::uint8_t row,
                                                                 std::uint8_t col,
                                                                 const utils::Direction &dir);

static bool is_on_board(const std::uint8_t row, const std::uint8_t col) {
    return row < utils::SIZE && col < utils::SIZE;
}


#endif  // OTHELLO_AI_STATE_H
