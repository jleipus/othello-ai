//
// Created by joris on 9/28/25.
//

#include "alphabetapruning.h"


#include "logger.h"

utils::Move AlphaBetaPruning::find_best_move(const State &state, const std::size_t depth,
                                             const std::chrono::steady_clock::time_point &start_time,
                                             std::chrono::milliseconds time_limit) {
    reset_stats();

    utils::Move best_move;
    try {
        minmax(state, depth, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), true,
               start_time, time_limit,
               &best_move);
    } catch (const TimeLimitExceeded &e) {}

    Logger::debug("Nodes evaluated: {}", nodes_evaluated);
    Logger::debug("Alpha cutoffs: {}", alpha_cutoffs);
    Logger::debug("Beta cutoffs: {}", beta_cutoffs);

    return best_move;
}

double AlphaBetaPruning::minmax(const State &state, const std::size_t depth, double alpha, double beta,
                                const bool maximizing_player, const std::chrono::steady_clock::time_point &start_time,
                                const std::chrono::milliseconds time_limit, utils::Move *best_move) {
    const auto now = std::chrono::steady_clock::now();
    if (now - start_time > time_limit) {
        throw TimeLimitExceeded();
    }

    nodes_evaluated++;
    if (depth == 0) {
        return heuristic->evaluate(state.get_board());
    }

    const auto possible_actions = get_legal_moves(state.get_board(), state.get_current_player());
    if (possible_actions.empty()) {
        return heuristic->evaluate(state.get_board());
    }

    double value;
    if (maximizing_player) {
        value = -std::numeric_limits<double>::infinity();
        for (const auto &move: possible_actions) {
            auto new_state = state.make_move(move);
            const auto new_value = minmax(*new_state, depth - 1, alpha, beta, false, start_time, time_limit, nullptr);
            if (new_value > value) {
                value = new_value;
                if (best_move) {
                    *best_move = move;
                }
            }
            alpha = std::max(alpha, value);
            if (beta <= alpha) {
                beta_cutoffs++;
                break; // Beta cut-off
            }
        }
    } else {
        value = std::numeric_limits<double>::infinity();
        for (const auto &move: possible_actions) {
            auto new_state = state.make_move(move);
            const auto new_value = minmax(*new_state, depth - 1, alpha, beta, true, start_time, time_limit, nullptr);
            if (new_value < value) {
                value = new_value;
                if (best_move) {
                    *best_move = move;
                }
            }
            beta = std::min(beta, value);
            if (beta <= alpha) {
                alpha_cutoffs++;
                break; // Alpha cut-off
            }
        }
    }

    return value;
}

void AlphaBetaPruning::reset_stats() {
    nodes_evaluated = 0;
    alpha_cutoffs = 0;
    beta_cutoffs = 0;
}

utils::Move iterative_deepening(const State &state, IHeuristic *heuristic, const std::size_t time_limit_ms) {
    Logger::debug("\n{}", state.get_board().to_string());

    const auto ab_pruning = new AlphaBetaPruning(heuristic);
    auto best_move = utils::Move(-1, -1, true); // Pass move by default

    const auto time_limit = std::chrono::milliseconds(time_limit_ms);
    const auto start_time = std::chrono::steady_clock::now();
    for (std::size_t depth = 1;; ++depth) {
        Logger::debug("Depth: {}", depth);

        const auto stage_start_time = std::chrono::steady_clock::now();
        auto move = ab_pruning->find_best_move(state, depth, start_time, time_limit);
        if (!move.is_empty()) {
            Logger::debug("Got move {}", best_move.to_string());
            best_move = move;
        } else {
            Logger::debug("No valid move found, stopping search.", depth);
            break;
        }

        const auto stage_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - stage_start_time).count();
        Logger::debug("Stage duration: {}ms\n", stage_duration);
    }

    const auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start_time).count();
    Logger::debug("Total time taken: {}ms\n", elapsed_time);
    return best_move;
}
