#define BOOST_TEST_MODULE "testGraph"

#include "graph.h"

#include <boost/test/included/unit_test.hpp>
#include <memory>

std::unique_ptr<Graph> setUp()
{
  return std::make_unique<Graph>("inputFiles/graph.txt");
}

// Tests the number of vertices are what they should be
BOOST_AUTO_TEST_CASE(testNumVertices)
{
  auto graph = setUp();
  BOOST_TEST(graph->getNumVertices() == 6);
}

// Tests all of the edges were created - bit complicated for a test but arguably necessary
BOOST_AUTO_TEST_CASE(testAllEdgesCreated)
{
  auto graph = setUp();
  BOOST_TEST(graph->getEdge(0, 1).has_value());
  BOOST_TEST(graph->getEdge(0, 3).has_value());
  BOOST_TEST(graph->getEdge(1, 2).has_value());
  BOOST_TEST(graph->getEdge(1, 4).has_value());
  BOOST_TEST(graph->getEdge(2, 5).has_value());
  BOOST_TEST(graph->getEdge(3, 2).has_value());
  BOOST_TEST(graph->getEdge(3, 4).has_value());
  BOOST_TEST(graph->getEdge(4, 3).has_value());
  BOOST_TEST(graph->getEdge(4, 5).has_value());
}