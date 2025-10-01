# libothello/CMakeLists.txt
set(LIB_NAME libothello)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LIB_SOURCES
        state.cpp
        state.h
        bitboard.cpp
        bitboard.h
        iheuristic.h
        countingevaluator.h
        utils.h
        alphabetapruning.cpp
        alphabetapruning.h
        logger.h
        cornerevaluator.h
        evaluator.cpp
        evaluator.h
        mobilityevaluator.h
        weightedevaluator.h
        stabilityevaluator.cpp
        stabilityevaluator.h)

add_library(${LIB_NAME} ${LIB_SOURCES})

target_include_directories(${LIB_NAME} PUBLIC ${SRC_DIR})

target_link_libraries(${LIB_NAME} PRIVATE spdlog::spdlog)
