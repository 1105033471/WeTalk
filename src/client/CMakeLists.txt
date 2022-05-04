add_executable(client)

target_sources(client PRIVATE client.cpp)

target_link_libraries(client PRIVATE wt_common)
