#pragma once

#include "edge.h"
#include "vertex.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Graph
{
private:
  // vertices in the graph, each contains adjacency list for faster traversals
  std::vector<std::shared_ptr<Vertex> > m_vertices;
  // adjacency matrix representation for fast lookups of edge weights
  std::vector<std::vector<std::unique_ptr<Edge> > > edges;
  // number of vertices in the graph
  int m_numVertices;

protected:
  template <typename ...Args>
  // To save having to make a few different versions to incorporate both lvalue and rvalue references :-)
  inline void emplaceEdge(Args&&...args)
  {
    addEdge(forward<Args>(args)...);
  }

  template <typename Weight>
  inline void addEdge(std::shared_ptr<Vertex>& v1, std::shared_ptr<Vertex>& v2, Weight&& weight)
  {
    edges[v1->getLabel()][v2->getLabel()] = make_unique<Edge>(v1, v2, weight);
  }

public:
  // Graph MUST be in format
  // numVertices
  // Space seperated matrix
  Graph(const char* filename)
  {
    std::ifstream iFile(filename);
    iFile >> m_numVertices;
    iFile.ignore(); // ignore new line character
    for (int i = 0; i < m_numVertices; ++i)
    {
      m_vertices.emplace_back(i);
    }
    edges.resize(m_numVertices);
    for (auto&& ve : edges)
    {
      ve.resize(m_numVertices);
    }
    std::string line;
    int vertex = 0;
    while (getline(iFile, line))
    {
      std::stringstream ss(line);
      int adjacentVertex = 0, edgeWeight;
      while (ss >> edgeWeight)
      {
        if (edgeWeight != 0)
        {
          emplaceEdge(m_vertices[vertex], m_vertices[adjacentVertex], edgeWeight);
        }
        else
        {
          edges[vertex][adjacentVertex] = nullptr;
        }
        ++adjacentVertex;
      }
      ++vertex;        
    }
  }

  inline std::shared_ptr<Vertex> const& getVertex(int label) const
  {
    return m_vertices[label];
  }

  inline Edge const& getEdge(Vertex const& v1, Vertex const &v2) const
  {
    return getEdge(v1.getLabel(), v2.getLabel());
  }

  inline Edge const& getEdge(int v1, int v2) const
  {
    return *edges[v1][v2];
  }

};
