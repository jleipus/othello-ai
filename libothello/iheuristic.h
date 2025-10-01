//
// Created by joris on 9/28/25.
//

#ifndef OTHELLO_AI_HEURISTIC_H
#define OTHELLO_AI_HEURISTIC_H
#include "bitboard.h"

class IHeuristic {
public:
    [[nodiscard]] virtual double evaluate(BitBoard board) const = 0;

    virtual ~IHeuristic() = default;
};

#endif  // OTHELLO_AI_HEURISTIC_H
