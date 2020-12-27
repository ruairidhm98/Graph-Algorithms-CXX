#pragma once

#include "graph.h"
#include <algorithm>
#include <list>
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
  bool isValidFlow()
  {
    const int numVertices = m_graph.getNumVertices();
    // Sum of incoming edges must be the same as sum of outgoing edges and flow must be > 0
    std::vector<int> outgoingFlow(numVertices), incomingFlow(numVertices);
    // Carry out full traversal of graph to compute outgoing and incoming flows for all v in V
    m_graph.visitEdges([this,&outgoingFlow,&incomingFlow](Edge const& edge)
    {
      int const& flow = edge.getFlow();
      int v1 = edge.getV1().getLabel(), v2 = edge.getV2().getLabel();
      if (flow < 0 || flow > edge.getWeight())
      {
        return false;
      }
      outgoingFlow[v1] = incomingFlow[v2] = flow;
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
      return 0;
    });
    return sum;
  }

  void augmentPath(std::list<std::reference_wrapper<Edge> > path)
  {   
    auto minEdge = std::min_element(path.begin(), path.end(),
      [](auto const& e1, auto const& e2)
      {
        return e1.get().getFlow() < e2.get().getFlow();
      });
    auto minFlow = minEdge->get().getFlow();
    for (auto&& e : path)
    {
      auto& edge = e.get();
      auto& optFwEdge = m_graph.getEdge(edge.getV1(), edge.getV2());
      if (optFwEdge.has_value() && (edge.getFlow() + minFlow <= edge.getWeight()))
      {
        auto& fwEdge = optFwEdge.value();
        fwEdge.setFlow(edge.getFlow() + minFlow);
      }
      else
      {
        auto& revEdge = m_graph.getEdge(edge.getV2(), edge.getV1()).value();
        revEdge.setFlow(revEdge.getFlow() - minFlow);
      }
    }
  }

  Graph& getGraph()
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

  void setSource(Vertex& source)
  {
    m_source = std::ref(source);
  }

  void setSink(Vertex& sink)
  {
    m_sink = std::ref(sink);
  }

  int getNumVertices()
  {
    return m_graph.getNumVertices();
  }

  Vertex& getVertex(int label)
  {
    return m_graph.getVertex(label);
  }

  template <typename Func>
  void visitEdges(Func&& func)
  {
    m_graph.visitEdges(std::forward<Func>(func));
  }

};