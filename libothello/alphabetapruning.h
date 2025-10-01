//
// Created by joris on 9/28/25.
//

#ifndef OTHELLO_AI_ALPHABETAPRUNING_H
#define OTHELLO_AI_ALPHABETAPRUNING_H

#include <memory>
#include <chrono>

#include "iheuristic.h"
#include "state.h"

struct TimeLimitExceeded : public std::exception {
    const char *what() const noexcept override {
        return "Time limit exceeded";
    }
};

class AlphaBetaPruning {
public:
    explicit AlphaBetaPruning(IHeuristic *heuristic) : heuristic(heuristic) {}

    utils::Move find_best_move(const State &state, std::size_t depth,
                               const std::chrono::steady_clock::time_point &start_time,
                               std::chrono::milliseconds time_limit);

private:
    IHeuristic *heuristic;

    std::size_t nodes_evaluated = 0;
    std::size_t alpha_cutoffs = 0;
    std::size_t beta_cutoffs = 0;

    double minmax(const State &state, std::size_t depth, double alpha, double beta, bool maximizing_player,
                  const std::chrono::steady_clock::time_point &start_time, std::chrono::milliseconds time_limit,
                  utils::Move *best_move);

    void reset_stats();
};

utils::Move iterative_deepening(const State &state, IHeuristic *heuristic, std::size_t time_limit_ms);

#endif  // OTHELLO_AI_ALPHABETAPRUNING_H
