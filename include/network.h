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
  // Graph might be initialised with default constructor so these won't be made yet. Use optional to avoid pointers
  std::optional<std::reference_wrapper<Vertex> > m_source;
  std::optional<std::reference_wrapper<Vertex> > m_sink;

  Network() = default;

  friend class ResidualGraph;

public:
  // Creates the graph from input filename
  explicit Network(const char* filename)
    : m_graph(filename)
    , m_source(std::ref(m_graph.getVertex(0)))
    , m_sink(std::ref(m_graph.getVertex(m_graph.getNumVertices()-1)))
  {}

  // Returns the true if the graph is a valid flow
  bool isValidFlow() const
  {
    const int numVertices = m_graph.getNumVertices();
    // Sum of incoming edges must be the same as sum of outgoing edges and flow must be > 0
    std::vector<int> outgoingFlow(numVertices), incomingFlow(numVertices);
    // Carry out full traversal of graph to compute outgoing and incoming flows for all v in V
    m_graph.visitEdges(
      [this,&outgoingFlow,&incomingFlow](Vertex const& v1, Vertex const& v2)
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
    auto&& source = m_source.value().get();
    int sum = 0;
    source.visitNeighbours([this,&sum,&source](Vertex& v)
    {
      sum += m_graph.getEdge(source, v).value().getFlow();
    });
    return sum;
  }

  Graph& getGraph()
  {
    return m_graph;
  }

  Graph const& getGraph() const
  {
    return m_graph;
  }

  Vertex& getSource()
  {
    return m_source.value().get();
  }

  Vertex& getSink()
  {
    return m_sink.value().get();
  }

  Vertex const& getSource() const
  {
    return m_source.value().get();
  }

  Vertex const& getSink() const
  {
    return m_sink.value().get();
  }

  void setSource(Vertex& source)
  {
    m_source = std::ref(source);
  }

  void setSink(Vertex& sink)
  {
    m_sink = std::ref(sink);
  }

};