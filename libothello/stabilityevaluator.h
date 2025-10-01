//
// Created by joris on 10/1/25.
//

#ifndef OTHELLO_AI_STABILITYEVALUATOR_H
#define OTHELLO_AI_STABILITYEVALUATOR_H
#include "iheuristic.h"


class StabilityEvaluator : public IHeuristic {
public:
    [[nodiscard]] double evaluate(BitBoard board) const override;

private:
    static constexpr int stable_score = 3;
    static constexpr int semi_stable_score = 1;

    static std::uint64_t get_stable_discs(std::uint64_t player_bits);

    static bool is_stable(std::uint8_t position, std::uint64_t player_bits, std::uint64_t stable_bits);

    static bool has_stable_connection_in_direction(std::uint8_t position, int direction,
                                                   std::uint64_t player_bits,
                                                   std::uint64_t stable_bits);

    static std::uint64_t get_semi_stable_discs(std::uint64_t player_bits, std::uint64_t stable_bits);

    static bool is_semi_stable(std::uint8_t position, std::uint64_t player_bits);
};


#endif //OTHELLO_AI_STABILITYEVALUATOR_H
