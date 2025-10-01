//
// Created by joris on 9/28/25.
//

#ifndef OTHELLO_AI_COUNTINGEVALUATOR_H
#define OTHELLO_AI_COUNTINGEVALUATOR_H
#include "iheuristic.h"

class CountingEvaluator : public IHeuristic {
public:
    [[nodiscard]] double evaluate(const BitBoard board) const override {
        const auto white_score = std::popcount(board.get_white_bitboard());
        const auto black_score = std::popcount(board.get_black_bitboard());

        if (white_score + black_score == 0) {
            return 0.0; // Avoid division by zero
        }

        return static_cast<double>(white_score - black_score) / (white_score + black_score);
    }
};

#endif  // OTHELLO_AI_COUNTINGEVALUATOR_H
