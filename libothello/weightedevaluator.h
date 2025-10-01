//
// Created by joris on 10/1/25.
//

#ifndef OTHELLO_AI_WEIGHTEDEVALUATOR_H
#define OTHELLO_AI_WEIGHTEDEVALUATOR_H
#include "bitboard.h"
#include "iheuristic.h"
#include "utils.h"


class WeightedEvaluator : public IHeuristic {
public:
    [[nodiscard]] double evaluate(const BitBoard board) const override {
        const auto white_bitmap = board.get_white_bitboard();
        const auto black_bitmap = board.get_black_bitboard();

        int white_score = 0;
        int black_score = 0;
        for (std::uint8_t row = 0; row < utils::SIZE; ++row) {
            for (std::uint8_t col = 0; col < utils::SIZE; ++col) {
                const auto bit_position = 1ULL << (row * utils::SIZE + col);
                if (white_bitmap & bit_position) {
                    white_score += weight_matrix[row][col];
                } else if (black_bitmap & bit_position) {
                    black_score += weight_matrix[row][col];
                }
            }
        }

        if (white_score + black_score == 0) {
            return 0.0; // Avoid division by zero
        }

        return static_cast<double>(white_score - black_score) / (white_score + black_score);
    }

private:
    inline static const std::array<std::array<int, 8>, 8> weight_matrix = {
        {
            {{4, -3, 2, 2, 2, 2, -3, 4}},
            {{-3, -4, -1, -1, -1, -1, -4, -3}},
            {{2, -1, 1, 0, 0, 1, -1, 2}},
            {{2, -1, 0, 1, 1, 0, -1, 2}},
            {{2, -1, 0, 1, 1, 0, -1, 2}},
            {{2, -1, 1, 0, 0, 1, -1, 2}},
            {{-3, -4, -1, -1, -1, -1, -4, -3}},
            {{4, -3, 2, 2, 2, 2, -3, 4}}
        }
    };
};


#endif //OTHELLO_AI_WEIGHTEDEVALUATOR_H
