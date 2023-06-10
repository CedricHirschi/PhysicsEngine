add_library(solver STATIC solver.hpp solver.cpp)
target_link_libraries(solver PUBLIC sfml-graphics ball)
target_include_directories(solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})