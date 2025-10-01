//
// Created by joris on 10/1/25.
//

#ifndef OTHELLO_AI_CORNEREVALUATOR_H
#define OTHELLO_AI_CORNEREVALUATOR_H
#include "iheuristic.h"


class CornerEvaluator : public IHeuristic {
public:
    [[nodiscard]] double evaluate(const BitBoard board) const override {
        const auto white_bitmap = board.get_white_bitboard();
        const auto black_bitmap = board.get_black_bitboard();

        int white_score = 0;
        int black_score = 0;
        for (const auto corner: utils::CORNERS) {
            const auto bit_position = 1ULL << corner;
            if (white_bitmap & bit_position) {
                white_score += corner_score;
            } else if (black_bitmap & bit_position) {
                black_score += corner_score;
            } else {
                for (const auto c_square: utils::C_SQUARES.at(corner)) {
                    const auto c_bit_position = 1ULL << c_square;
                    if (white_bitmap & c_bit_position) {
                        white_score += c_square_score;
                    } else if (black_bitmap & c_bit_position) {
                        black_score += c_square_score;
                    }
                }

                const auto x_square = utils::X_SQUARES.at(corner);
                const auto x_bit_position = 1ULL << x_square;
                if (white_bitmap & x_bit_position) {
                    white_score += x_square_score;
                } else if (black_bitmap & x_bit_position) {
                    black_score += x_square_score;
                }
            }
        }

        if (white_score + black_score == 0) {
            return 0.0; // Avoid division by zero
        }

        return static_cast<double>(white_score - black_score) / (white_score + black_score);
    }

private:
    static constexpr int corner_score = 5;
    static constexpr int c_square_score = -1;
    static constexpr int x_square_score = -3;
};

#endif //OTHELLO_AI_CORNEREVALUATOR_H
