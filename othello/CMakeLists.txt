# othello/CMakeLists.txt
add_executable(othello main.cpp)
target_link_libraries(othello PRIVATE libothello)
target_link_libraries(othello PRIVATE spdlog::spdlog)