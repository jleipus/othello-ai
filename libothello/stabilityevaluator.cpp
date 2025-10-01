//
// Created by joris on 10/1/25.
//

#include "stabilityevaluator.h"
#include <algorithm>

double StabilityEvaluator::evaluate(const BitBoard board) const {
    const auto white_bits = board.get_white_bitboard();
    const auto black_bits = board.get_black_bitboard();

    const auto white_stable_bits = get_stable_discs(white_bits);
    const auto black_stable_bits = get_stable_discs(black_bits);

    const auto white_semi_stable_bits = get_semi_stable_discs(white_bits, white_stable_bits);
    const auto black_semi_stable_bits = get_semi_stable_discs(black_bits, black_stable_bits);

    const auto white_stable_count = std::popcount(white_stable_bits);
    const auto black_stable_count = std::popcount(black_stable_bits);

    const auto white_semi_stable_count = std::popcount(white_semi_stable_bits);
    const auto black_semi_stable_count = std::popcount(black_semi_stable_bits);

    const auto white_score = white_stable_count * stable_score + white_semi_stable_count * semi_stable_score;
    const auto black_score = black_stable_count * stable_score + black_semi_stable_count * semi_stable_score;

    if (white_score + black_score == 0) {
        return 0.0; // Avoid division by zero
    }

    return static_cast<double>(white_score - black_score) / (white_score + black_score);
}

std::uint64_t StabilityEvaluator::get_stable_discs(std::uint64_t player_bits) {
    std::uint64_t stable_bits = 0;

    // Check corners
    for (const auto corner: utils::CORNERS) {
        const auto bit_position = 1ULL << corner;
        if (player_bits & bit_position) {
            stable_bits |= bit_position;
        }
    }

    if (stable_bits == 0) {
        return 0; // No stable discs if no corners are occupied
    }

    bool updated;
    do {
        updated = false;
        for (std::uint8_t pos = 0; pos < utils::SIZE * utils::SIZE; ++pos) {
            const auto bit_position = 1ULL << pos;
            if (!(player_bits & bit_position) || (stable_bits & bit_position)) {
                continue; // Skip if not player's disc or already stable
            }

            if (is_stable(pos, player_bits, stable_bits)) {
                stable_bits |= bit_position;
                updated = true;
            }
        }
    } while (updated);

    return stable_bits;
}

bool StabilityEvaluator::is_stable(const std::uint8_t position, const std::uint64_t player_bits,
                                   const std::uint64_t stable_bits) {
    for (const auto &direction: utils::DIRECTIONS) {
        if (!has_stable_connection_in_direction(position, direction.to_shift(), player_bits, stable_bits)) {
            return false;
        }
    }
    return true;
}

bool StabilityEvaluator::has_stable_connection_in_direction(const std::uint8_t position, const int direction,
                                                            const std::uint64_t player_bits,
                                                            const std::uint64_t stable_bits) {
    auto x = position + direction;

    while (x >= 0 && x < utils::SIZE * utils::SIZE) {
        const auto bit_position = 1ULL << x;
        if (!(player_bits & bit_position)) {
            return false; // Encountered an empty square or opponent's disc
        }

        if (stable_bits & bit_position) {
            return true; // Found a stable disc in this direction
        }

        x += direction;
    }

    if (std::ranges::find(std::begin(utils::EDGES), std::end(utils::EDGES), position) != std::end(utils::EDGES)) {
        return true; // Current position is on the edge
    }

    return true; // Reached the edge of the board
}

std::uint64_t StabilityEvaluator::get_semi_stable_discs(const std::uint64_t player_bits,
                                                        const std::uint64_t stable_bits) {
    std::uint64_t semi_stable_bits = 0;

    for (std::uint8_t pos = 0; pos < utils::SIZE * utils::SIZE; ++pos) {
        const auto bit_position = 1ULL << pos;
        if (!(player_bits & bit_position) || (stable_bits & bit_position)) {
            continue; // Skip if not player's disc or already stable
        }

        if (std::ranges::find(std::begin(utils::EDGES), std::end(utils::EDGES), pos) != std::end(utils::EDGES)) {
            semi_stable_bits |= bit_position;
            continue;
        }

        if (is_semi_stable(pos, player_bits)) {
            semi_stable_bits |= bit_position;
        }
    }

    return semi_stable_bits;
}

bool StabilityEvaluator::is_semi_stable(const std::uint8_t position, const std::uint64_t player_bits) {
    std::uint8_t protected_directions = 0;
    for (const auto &direction: utils::DIRECTIONS) {
        const auto neighbor_pos = position + direction.to_shift();

        if (neighbor_pos < 0 || neighbor_pos >= utils::SIZE * utils::SIZE) {
            protected_directions++;
            continue; // Out of bounds, consider it protected
        }

        const auto bit_position = 1ULL << neighbor_pos;
        if (player_bits & bit_position) {
            protected_directions++;
        }
    }

    return protected_directions >= 3;
}
