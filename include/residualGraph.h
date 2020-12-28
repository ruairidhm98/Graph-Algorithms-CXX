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
    int cursor = m_net.getSink()->getLabel();
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
    auto&& source = m_net.getSource();
    queue.push(std::ref(*source));
    visited[source->getLabel()] = 1;

    bool done = false;
    // Carry out a breadth first search looking for a path from the source to the sink
    while (queue.size() > 0 && !done)
    {
      Vertex& u = queue.front().get();
      queue.pop();
      int uLabel = u.getLabel();
      for (auto&& ptr : u.getNeighbours())
      {
        if (auto v = ptr.lock())
        {
          int vLabel = v->getLabel();
          if (!visited[vLabel])
          {
            visited[vLabel] = 1;
            predecessor[vLabel] = uLabel;
            // Path has been found, return true
            if (vLabel == m_net.getSink()->getLabel())
            {
              done = true;
              break;
            }
            queue.push(*v);
          }
        }
      }   
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

    m_net.setSource(resGraph.getVertex(0));
    m_net.setSink(resGraph.getVertex(resGraph.getNumVertices()-1));
    g.visitEdges([&](Edge const& e)
    {
      // If we can still push flow through the edge and if the edge has not already been created
      int v1 = e.getV1().getLabel(), v2 = e.getV2().getLabel();
      if (e.getFlow() < e.getWeight() && !m_net.getGraph().getEdge(v1, v2).has_value())
      {
        m_net.getGraph().addEdge(v1, v2, e.getWeight() - e.getFlow());
      }
      // If we can 'reverse' the flow of the edge and if the edge has not already been created
      if (e.getFlow() > 0 && !m_net.getGraph().getEdge(v2, v1).has_value())
      {
        m_net.getGraph().addEdge(v2, v1, e.getFlow());
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
