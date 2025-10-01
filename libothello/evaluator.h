//
// Created by joris on 10/1/25.
//

#ifndef OTHELLO_AI_EVALUATOR_H
#define OTHELLO_AI_EVALUATOR_H
#include <memory>

#include "bitboard.h"
#include "iheuristic.h"


class Evaluator : public IHeuristic {
public:
    Evaluator();

    [[nodiscard]] double evaluate(BitBoard board) const override;

private:
    std::map<std::string, std::unique_ptr<IHeuristic> > heuristics;

    [[nodiscard]] static std::map<std::string, int> get_dynamic_weights(std::uint8_t empty_squares);
};


#endif //OTHELLO_AI_EVALUATOR_H
