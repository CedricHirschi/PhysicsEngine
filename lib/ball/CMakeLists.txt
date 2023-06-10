add_library(ball STATIC ball.hpp ball.cpp)
target_link_libraries(ball PUBLIC sfml-graphics)
target_include_directories(ball PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})