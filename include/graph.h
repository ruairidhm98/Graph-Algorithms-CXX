#pragma once

#include "edge.h"
#include "vertex.h"

#include <fstream>
#include <optional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Graph
{
private:
  // vertices in the graph, each contains adjacency list for faster traversals
  std::vector<std::shared_ptr<Vertex> > m_vertices;
  // adjacency matrix representation for fast lookups of edge weights, use optional to avoid null pointers
  std::vector<std::vector<std::optional<Edge> > > m_edges;
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
    m_edges[v1->getLabel()][v2->getLabel()] = move(Edge(v1, v2, weight));
  }

  // Reads the file
  void createEdges(std::ifstream&& iFile)
  {
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
          m_vertices[vertex]->addNeighbour(m_vertices[adjacentVertex]);
        }
        ++adjacentVertex;
      }
      ++vertex;        
    }
  }

public:

  Graph(const char* filename)
  {
    std::ifstream iFile(filename);
    iFile >> m_numVertices;
    iFile.ignore();
    m_edges.resize(m_numVertices);
    for (int i = 0; i < m_numVertices; ++i)
    {
      m_vertices.emplace_back(i);
      m_edges[i].resize(m_numVertices);
    }
    
    createEdges(move(iFile));
  }

  inline Vertex const& getVertex(int label) const
  {
    return *m_vertices[label];
  }

  inline int getNumVertices() const
  {
    return m_numVertices;
  }

  inline std::optional<Edge> const& getEdge(Vertex const& v1, Vertex const &v2) const
  {
    return getEdge(v1.getLabel(), v2.getLabel());
  }

  inline std::optional<Edge> const& getEdge(int v1, int v2) const
  {
    return m_edges[v1][v2];
  }

};
