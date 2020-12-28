#include "edge.h"
#include "graph.h"
#include "vertex.h"

#include <functional>
#include <iostream>
#include <queue>
#include <vector>


int dijktras(Graph& g, int source, int end)
{
  // Use a priority queue to keep track of the vertex with smallest distance
  std::priority_queue<std::reference_wrapper<Vertex> > minDv;
  // Keep track of who is visited, this should be a bitset
  std::vector<bool> visited(g.getNumVertices());
  // Predecessor array to keep track of actual path
  std::vector<int> predecessor(g.getNumVertices()), dist(g.getNumVertices());

  // Initialisation
  for (int i = 0; i < g.getNumVertices(); ++i)
  {
    if (i != source)
    {
      auto& optEdge = g.getEdge(source, i);
      if (optEdge.has_value())
      {
        auto& edge = optEdge.value();
        dist[i] = edge.getWeight();
      }
      else
      {
        dist[i] = INT_MAX;
      }
    }
  }

  predecessor[source] = -1;
  minDv.push(*g.getVertex(source));


}

int main(int argc, char** argv)
{
  if (argc != 4)
  {
    std::cerr << "Please provide 3 arguments, inputFile, source and target vertices";
    return -1;
  }

  Graph g(argv[1]);
  int source = std::atoi(argv[2]), target = std::atoi(argv[3]);
  dijktras(g, source, end);


  return 0;
}