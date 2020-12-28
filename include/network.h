#pragma once

#include "graph.h"
#include <algorithm>
#include <list>
#include <memory>
#include <numeric>
#include <vector>

// A network is a directed graph in which edges have capacity and flow. The design intentionally avoids
// inheritance from Graph to avoid (A) complexity and (B) dynamic dispatch/virtual function overhead
class Network
{
private:
  Graph m_graph;
  std::weak_ptr<Vertex> m_source;
  std::weak_ptr<Vertex> m_sink;

  Network() = default;

  void setSource(std::shared_ptr<Vertex> source)
  {
    m_source = source;
  }

  void setSink(std::shared_ptr<Vertex> sink)
  {
    m_sink = sink;
  }

  friend class ResidualGraph;

public:
  // Creates the graph from input filename
  explicit Network(const char* filename)
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
    int sum = 0;
    if (auto source = m_source.lock())
    {
      source->visitNeighbours([this,&sum,&source](Vertex& v)
      {
        sum += m_graph.getEdge(*source, v).value().getFlow();
        return 1;
      });
    }
    return sum;
  }

  void augmentPath(std::list<std::reference_wrapper<Edge> > path)
  {   
    auto minEdge = std::min_element(path.begin(), path.end(),
      [](auto const& e1, auto const& e2)
      {
        return e1.get().getWeight() < e2.get().getWeight();
      });
    int minFlow = minEdge->get().getWeight();
    for (auto&& e : path)
    {
      auto&& edge = e.get();
      
      auto&& optFwEdge = m_graph.getEdge(edge.getV1(), edge.getV2());
      if (optFwEdge.has_value() && (edge.getFlow() + minFlow <= edge.getWeight()))
      {
        auto&& fwEdge = optFwEdge.value();
        fwEdge.setFlow(fwEdge.getFlow() + minFlow);
      }
      else
      {
        auto&& revEdge = m_graph.getEdge(edge.getV2(), edge.getV1()).value();
        revEdge.setFlow(revEdge.getFlow() - minFlow);
      }
    }
  }

  Graph& getGraph()
  {
    return m_graph;
  }

  std::shared_ptr<Vertex> getSource()
  {
    return m_source.lock();
  }

  std::shared_ptr<Vertex> getSink()
  {
    return m_sink.lock();
  }

  int getNumVertices()
  {
    return m_graph.getNumVertices();
  }

  Vertex& getVertex(int label)
  {
    return *m_graph.getVertex(label);
  }

  template <typename Func>
  void visitEdges(Func&& func)
  {
    m_graph.visitEdges(std::forward<Func>(func));
  }

  void print()
  {
    m_graph.visitEdges([](Edge const& e)
    {
      e.print(' ');
      std::cout << e.getFlow() << "(" << e.getWeight() << ")\n";
      return true;
    });
  }

};