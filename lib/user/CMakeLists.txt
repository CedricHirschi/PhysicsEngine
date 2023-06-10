add_library(user STATIC user.hpp user.cpp)
target_link_libraries(user PUBLIC sfml-graphics solver)
target_include_directories(user PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})