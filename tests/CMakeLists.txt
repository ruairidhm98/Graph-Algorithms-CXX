
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})

add_executable(testGraph testGraph.cpp)
target_link_libraries(testGraph LINK_PUBLIC ${Boost_LIBRARIES})
add_test(graph testGraph)

add_executable(testNetwork testNetwork.cpp)
target_link_libraries(testNetwork LINK_PUBLIC ${Boost_LIBRARIES})
add_test(network testNetwork)
