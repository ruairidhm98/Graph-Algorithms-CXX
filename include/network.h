#pragma once

#include "graph.h"
#include <algorithm>
#include <numeric>
#include <vector>

// A network is a directed graph in which edges have capacity and flow. The design intentionally avoids
// inheritance from Graph to avoid (A) complexity and (B) dynamic dispatch/virtual function overhead
class Network
{
private:
  Graph m_graph;
  Vertex const& m_source;
  Vertex const& m_sink;
public:
  // Assumes same graph format
  Network(const char* filename)
    : m_graph(filename)
    , m_source(m_graph.getVertex(0))
    , m_sink(m_graph.getVertex(m_graph.getNumVertices()-1))
  {}

  // Returns the true if the graph is a valid flow
  bool isValidFlow()
  {
    const int numVertices = m_graph.getNumVertices();
    // Sum of incoming edges must be the same as sum of outgoing edges and flow must be > 0
    std::vector<int> outgoingFlow(numVertices), incomingFlow(numVertices);
    // Carry out full traversal of graph to compute outgoing and incoming flows for all v in V
    m_graph.visitVertices([this,&outgoingFlow,&incomingFlow](Vertex &v1, Vertex &v2)
    {
      auto const& edge = m_graph.getEdge(v1, v2).value();
      int const& flow = edge.getFlow();
      if (flow < 0 || flow > edge.getWeight())
      {
        return false;
      }
      outgoingFlow[v1.getLabel()] = incomingFlow[v2.getLabel()] = flow;
      return true;
    });

    return std::equal(outgoingFlow.begin()+1, outgoingFlow.begin()+numVertices-1, incomingFlow.begin());
  }

  // Returns the flow in the network. This is the sum of outgoing flows from the source vertex
  int getFlow() const
  {
    auto&& nSource = m_source.getNeighbours();
    return std::accumulate(nSource.begin(), nSource.end(), 0, [](auto&& v)
    {
      if (auto ptr = ve.lock())
      {
        sum += m_graph.getEdge(m_source, *ptr).value().getFlow();
      }
    });
  }

};