//
// Created by joris on 10/1/25.
//

#include "evaluator.h"

#include "cornerevaluator.h"
#include "mobilityevaluator.h"
#include "stabilityevaluator.h"
#include "countingevaluator.h"
#include "weightedevaluator.h"

Evaluator::Evaluator() {
    heuristics["corner"] = std::make_unique<CornerEvaluator>();
    heuristics["mobility"] = std::make_unique<MobilityEvaluator>();
    heuristics["stability"] = std::make_unique<StabilityEvaluator>();
    heuristics["counting"] = std::make_unique<CountingEvaluator>();
    heuristics["weighted"] = std::make_unique<WeightedEvaluator>();
}

double Evaluator::evaluate(const BitBoard board) const {
    const auto empty_squares = utils::SIZE * utils::SIZE - std::popcount(
                                   board.get_black_bitboard() | board.get_white_bitboard());
    const auto weights = get_dynamic_weights(empty_squares);

    double total_score = 0.0;
    for (const auto &[name, heuristic]: heuristics) {
        const auto weight = weights.at(name);
        total_score += weight * heuristic->evaluate(board);
    }

    return total_score;
}

std::map<std::string, int> Evaluator::get_dynamic_weights(const std::uint8_t empty_squares) {
    std::map<std::string, int> weights;
    if (empty_squares > 40) {
        // Opening
        weights["corner"] = 25;
        weights["mobility"] = 50;
        weights["stability"] = 5;
        weights["counting"] = 0;
        weights["weighted"] = 50;
    } else if (empty_squares > 20) {
        // Mid-game
        weights["corner"] = 75;
        weights["mobility"] = 30;
        weights["stability"] = 20;
        weights["counting"] = 5;
        weights["weighted"] = 30;
    } else {
        // Endgame
        weights["corner"] = 200;
        weights["mobility"] = 5;
        weights["stability"] = 50;
        weights["counting"] = 20;
        weights["weighted"] = 5;
    }

    return weights;
}
