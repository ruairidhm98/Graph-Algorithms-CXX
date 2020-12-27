#define BOOST_TEST_MODULE "testGraph"

#include "graph.h"

#include <boost/test/included/unit_test.hpp>
#include <memory>
#include <string>

using namespace std::string_literals;

std::unique_ptr<Graph> setUp()
{
  return std::make_unique<Graph>("graph.txt"s);
}

BOOST_AUTO_TEST_CASE(testCreation)
{
  auto graph = setUp();
  BOOST_TEST(graph->getNumVertices() == 6);
}
