#pragma once

#include <list>
#include <queue>
#include <vector>

class Network;

class ResidualGraph
{
private:
  Network m_net;

  std::list<std::reference_wrapper<Edge> > tracePath(std::vector<int>&& predecessor);
  std::vector<int> findPath();

public:
  explicit ResidualGraph(Network& net);
  std::list<std::reference_wrapper<Edge> > findAugmentingPath();

};
