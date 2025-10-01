//
// Created by joris on 10/1/25.
//

#ifndef OTHELLO_AI_MOBILITYEVALUATOR_H
#define OTHELLO_AI_MOBILITYEVALUATOR_H
#include "iheuristic.h"
#include "state.h"


class MobilityEvaluator : public IHeuristic {
public:
    [[nodiscard]] double evaluate(const BitBoard board) const override {
        const auto white_score = get_legal_moves(board, utils::Piece::White).size();
        const auto black_score = get_legal_moves(board, utils::Piece::Black).size();

        if (white_score + black_score == 0) {
            return 0.0; // Avoid division by zero
        }

        return static_cast<double>(white_score - black_score) / (white_score + black_score);
    }
};


#endif //OTHELLO_AI_MOBILITYEVALUATOR_H
