#include "edge.h"
#include "graph.h"
#include "network.h"
#include "residualGraph.h"

#include <list>
#include <queue>
#include <vector>

ResidualGraph::ResidualGraph(Network& net)
{
  // Initialise the vertices and edges of the graph
  m_net.initialiseGraph(net.getGraph().getNumVertices());
  
  Graph& g = net.getGraph(), &resGraph = m_net.getGraph();
  g.visitEdges([&](Edge const& e)
  {
    // If we can still push flow through the edge and if the edge has not already been created
    int v1 = e.getV1().getLabel(), v2 = e.getV2().getLabel();
    if (e.getFlow() < e.getWeight() && !resGraph.getEdge(v1, v2).has_value())
    {
      resGraph.addEdge(v1, v2, e.getWeight() - e.getFlow());
    }
    // If we can 'reverse' the flow of the edge and if the edge has not already been created
    if (e.getFlow() > 0 && !resGraph.getEdge(v2, v1).has_value())
    {
      resGraph.addEdge(v2, v1, e.getFlow());
    }
    return true;
  });
}

std::list<std::reference_wrapper<Edge> > ResidualGraph::tracePath(std::vector<int> predecessor)
{
  std::list<std::reference_wrapper<Edge> > path;
  int cursor = m_net.getSink()->getLabel();
  while (cursor != -1 && predecessor[cursor] != -1)
  {
    path.push_front(std::ref(m_net.getGraph().getEdge(predecessor[cursor], cursor).value()));
    cursor = predecessor[cursor];
  }

  return path;
}

std::vector<int> ResidualGraph::findPath()
{
  Graph& resGraph = m_net.getGraph();
  std::vector<bool> visited(resGraph.getNumVertices());
  std::vector<int> predecessor(resGraph.getNumVertices(), -1);

  std::queue<std::reference_wrapper<Vertex> > queue;
  auto&& source = m_net.getSource();
  queue.push(std::ref(*source));
  visited[source->getLabel()] = 1;

  bool done = false;
  // Carry out a breadth first search looking for a path from the source to the sink
  while (queue.size() > 0 && !done)
  {
    Vertex& u = queue.front().get();
    queue.pop();
    u.visitNeighbours([&](Vertex &v)
    {
      int vLabel = v.getLabel(), uLabel = u.getLabel();
      if (!visited[vLabel])
      {
        visited[vLabel] = 1;
        predecessor[vLabel] = uLabel;
        if (vLabel == m_net.getSink()->getLabel())
        {
          done = true;
          return false;
        }
        queue.push(v);
      }
      return true;
    });
  }
  return predecessor;
}

std::list<std::reference_wrapper<Edge> > ResidualGraph::findAugmentingPath()
{
  auto predecessor = findPath();
  return tracePath(std::move(predecessor));
}
