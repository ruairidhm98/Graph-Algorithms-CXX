#define BOOST_TEST_MODULE "testNetwork"

#include "network.h"

#include <boost/test/included/unit_test.hpp>
#include <memory>

std::unique_ptr<Network> setUp()
{
  return std::make_unique<Network>("inputFiles/graph.txt");
}

BOOST_AUTO_TEST_CASE(testValidFlow)
{
  auto network = setUp();
  // Should be a valid flow
  BOOST_TEST(network->isValidFlow());
}

