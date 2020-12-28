#include "edge.h"
#include "graph.h"
#include "vertex.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

// Just using a template here for the queue to avoid writing the extensive amount of parameters
template <typename Map>
void initialise(Graph& g, std::vector<int>& dist, int& source, Map&& pq)
{
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
    pq.insert({i, dist[i]});
  }
}

void printPath(int curVertex, std::vector<int> predecessor)
{
  if (curVertex == -1)
  {
    return;
  }
  printPath(predecessor[curVertex], std::move(predecessor));
  std::cout << curVertex << " ";
}

void outputSolution(std::vector<int> predecessor, std::vector<int> dist, int& source, int& target)
{
  // If there is a path from source to target
  if (dist[target] != INT_MAX && target < dist.size())
  {   
    std::cout << "Shortest distance from " << source << " to " << target;
    std::cout << ": " << dist[target] << std::endl;
    std::cout << "Shortest path: ";
    printPath(target, std::move(predecessor));
    std::cout << std::endl;
  }
  else
  {
    std::cout << "No such path exists" << std::endl;
  }
  
}

void dijktras(Graph& g, int source, int target)
{
  // Keep track of who is visited, this should be a bitset
  std::vector<bool> visited(g.getNumVertices());
  // Predecessor array to keep track of actual path
  std::vector<int> predecessor(g.getNumVertices()), dist(g.getNumVertices());

  auto cmp = [&](auto const& v1, auto const& v2)
  {
    return dist[v1] < dist[v2];
  };

  using VertexRef = std::reference_wrapper<Vertex>;

  // Use a map to keep track of the vertex with smallest distance
  std::map<int,int,decltype(cmp)> minDv(cmp);

  initialise(g, dist, source, minDv);

  predecessor[source] = -1;
  while (minDv.size() > 0)
  {
    auto pos = minDv.begin();
    int i = pos->first;
    auto& v = *g.getVertex(i);
    minDv.erase(pos);

    v.visitNeighbours([&](Vertex& u)
    {
      auto& edge = g.getEdge(v, u).value();
      int uLabel = u.getLabel(), vLabel = v.getLabel(), edgeWeight = edge.getWeight();
      if (dist[v.getLabel()] != INT_MAX && edgeWeight + dist[vLabel] < dist[uLabel])
      {
        dist[uLabel] = edgeWeight + dist[vLabel];
        predecessor[uLabel] = vLabel;
        // Need to re-order priority queue
        minDv[uLabel] = dist[uLabel];
      }
      return true;
    });
    // Target vertex has been found
    if (v.getLabel() == target)
    {
      break;
    }
  }

  outputSolution(std::move(predecessor), std::move(dist), source, target);
}


int main(int argc, char** argv)
{
  if (argc != 4)
  {
    std::cerr << "Please provide 3 arguments, inputFile, source and target vertices" << std::endl;
    return -1;
  }

  Graph g(argv[1]);
  int source = std::atoi(argv[2]), target = std::atoi(argv[3]);
  dijktras(g, source, target);

  return 0;
}