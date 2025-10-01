#include <iostream>

#include "alphabetapruning.h"
#include "countingevaluator.h"
#include "evaluator.h"
#include "logger.h"
#include "state.h"


std::string usage() {
    return "Usage: <board_string> <time_limit_s>\n"
            "[--white_char <char>] White player character (default: O)\n"
            "[--black_char <char>] Black player character (default: X)\n"
            "[--empty_char <char>] Empty cell character (default: E)\n"
            "[--log_level <level>] Logging level (trace: 0, debug: 1, info: 2, warn: 3, err: 4, off: 6)\n";
}


int main(const int argc, char *argv[]) {
    std::string board_str;
    int time_limit_s = 0;

    auto white_char = 'W';
    auto black_char = 'B';
    auto empty_char = 'E';

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--white_char" && i + 1 < argc) {
            white_char = argv[++i][0];
        } else if (arg == "--black_char" && i + 1 < argc) {
            black_char = argv[++i][0];
        } else if (arg == "--empty_char" && i + 1 < argc) {
            empty_char = argv[++i][0];
        } else if (arg == "--log_level" && i + 1 < argc) {
            const auto log_level = std::stoi(argv[++i]);
            Logger::set_level(log_level);
        } else if (board_str.empty()) {
            board_str = argv[i];
        } else if (time_limit_s == 0) {
            time_limit_s = std::stoi(argv[i]);
        } else {
            std::cerr << "Unknown argument: " << arg << "\n";
            return EXIT_FAILURE;
        }
    }

    if (board_str.empty() || time_limit_s == 0) {
        std::cout << usage();
        return EXIT_FAILURE;
    }

    const State initial_state(board_str, white_char, black_char, empty_char);
    const auto best_move = iterative_deepening(initial_state, new Evaluator(), time_limit_s * 1000);
    std::cout << best_move.to_string() << std::endl;
}
