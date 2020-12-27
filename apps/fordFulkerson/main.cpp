#include "network.h"
#include "residualGraph.h"

#include <iostream>

void fordFulkerson(Network& net)
{
  // Traverse all the edges setting flow to 0
  net.visitEdges([](Edge &e)
  {
    e.setFlow(0);
    return true;
  });

  // Repetedly build a residual graph from the network, seraching for directed paths from the
  // source to the sink in the residual graph. If one is found, then augment this path to push
  // more flow through the edges (or decrease if backwards edges in the path). If none is found
  // then we have found a maximum flow as we can't push anymore flow through the network
  for (;;)
  {
    ResidualGraph gPrime(net);
    auto path = gPrime.findAugmentingPath();
    if (path.size() > 0)
    {
      net.augmentPath(path);
    }
    else
    {
      break;
    }
  }
}

  

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Please provide input file" << std::endl;
    return 1;
  }

  Network net(argv[1]);
  fordFulkerson(net);
  std::cout << "Maximum flow = " << net.getFlow() << std::endl;

  return 0;
}