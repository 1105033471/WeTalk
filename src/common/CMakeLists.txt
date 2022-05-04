add_library(wt_common STATIC)

target_sources(wt_common PRIVATE wt_socket.cpp)

target_include_directories(wt_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(wt_common PUBLIC ws2_32)
