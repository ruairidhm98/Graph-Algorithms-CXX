#pragma once

#include "graph.h"

#include <list>
#include <memory>

// A network is a directed graph in which edges have capacity and flow.
// The design intentionally avoids inheritance from Graph 
class Network
{
private:
  Graph m_graph;
  std::weak_ptr<Vertex> m_source;
  std::weak_ptr<Vertex> m_sink;

  Network();
  void initialiseGraph(int numVertices);

  friend class ResidualGraph;

public:

  explicit Network(const char* filename);
  bool isValidFlow();
  int getFlow() const;
  void augmentPath(std::list<std::reference_wrapper<Edge> > path);
  Graph& getGraph();
  std::shared_ptr<Vertex> getSource();
  std::shared_ptr<Vertex> getSink();
  int getNumVertices();
  Vertex& getVertex(int label);
  void print();

  template <typename Func>
  void visitEdges(Func&& func)
  {
    m_graph.visitEdges(std::forward<Func>(func));
  }

};