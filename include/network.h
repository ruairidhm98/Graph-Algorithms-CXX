#pragma once

#include "graph.h"

// A network is a directed graph in which edges have capacity. The design explicitly avoids
// inheritance to avoid (A) complexity and (B) dynamic dispatch/virtual function overhead
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

  

};