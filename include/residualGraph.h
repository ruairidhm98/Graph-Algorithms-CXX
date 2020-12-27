#pragma once

#include "network.h"
#include <list>
#include <queue>
#include <vector>

class ResidualGraph
{
private:
  Network m_net;

  std::list<std::reference_wrapper<Edge> > tracePath(std::vector<int>&& predecessor)
  {
    std::list<std::reference_wrapper<Edge> > path;
    int cursor = m_net.getSink().getLabel();
    while (cursor != -1 && predecessor[cursor] != -1)
    {
      path.push_front(std::ref(m_net.getGraph().getEdge(predecessor[cursor], cursor).value()));
      cursor = predecessor[cursor];
    }
    return path;
  }

  std::vector<int> findPath()
  {
    Graph& resGraph = m_net.getGraph();
    std::vector<bool> visited(resGraph.getNumVertices());
    std::vector<int> predecessor(resGraph.getNumVertices(), -1);

    std::queue<std::reference_wrapper<Vertex> > queue;
    Vertex& source = m_net.getSource();
    queue.push(std::ref(source));
    visited[source.getLabel()] = 1;
    // Carry out a breadth first search looking for a path from the source to the sink
    while (queue.size() > 0)
    {
      Vertex& u = queue.front().get();
      queue.pop();
      u.visitNeighbours([&](Vertex& v)
      {
        int vLabel = v.getLabel(), uLabel = u.getLabel();
        if (!visited[vLabel])
        {
          visited[vLabel] = 1;
          predecessor[vLabel] = uLabel;
          // Path has been found, return true
          if (vLabel == m_net.getSink().getLabel())
          {
            return true;
          }
          queue.push(std::ref(v));
        }
        return false;
      });    
    }
    return predecessor;
  }

public:
  ResidualGraph(Network& net)
  {
    // Initialise the vertices and edges of the graph
    Graph& resGraph = m_net.getGraph();
    resGraph.initialiseGraph(net.getGraph().getNumVertices());
    
    Graph& g = net.getGraph();
    m_net.setSource(g.getVertex(0));
    m_net.setSink(g.getVertex(g.getNumVertices()-1));
    g.visitEdges([this,&g,&resGraph](Edge const& e)
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
        resGraph.addEdge(v2, v1, std::move(e.getFlow()));
      }
      return true;
    });
  }

  std::list<std::reference_wrapper<Edge> > findAugmentingPath()
  {
    auto predecessor = findPath();
    return tracePath(std::move(predecessor));
  }


};
