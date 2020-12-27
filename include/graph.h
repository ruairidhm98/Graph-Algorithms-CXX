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

  friend class ResidualGraph;

protected:

  // Reads the file
  void createEdges(std::ifstream&& iFile)
  {
    std::string line;
    int vertex = 0;
    while (std::getline(iFile, line))
    {
      std::stringstream ss(line);
      int adjacentVertex = 0, edgeWeight;
      char b;
      // Line will be in format e.g. 1(2), vertex 0 has edge to vertex 1 of weight 2
      while (ss >> adjacentVertex >> b >> edgeWeight >> b)
      {
        addEdge(m_vertices[vertex], m_vertices[adjacentVertex], std::move(edgeWeight));
        m_vertices[vertex]->addNeighbour(m_vertices[adjacentVertex]);
      }
      ++vertex;        
    }
  }

  void initialiseGraph(int numVertices)
  {
    m_numVertices = numVertices;
    m_edges.resize(numVertices);
    for (int i = 0; i < m_numVertices; ++i)
    {
      m_vertices.push_back(std::make_shared<Vertex>(i));
      m_edges[i].resize(m_numVertices);
    }
  }

  inline void addEdge(std::shared_ptr<Vertex>& v1, std::shared_ptr<Vertex>& v2, int weight)
  {
    m_edges[v1->getLabel()][v2->getLabel()] = std::move(Edge(v1, v2, weight));
  }

public:

  // Assume file is in format
  // line 0| first line is an integer containing the number of vertices
  // line i| neighbour_v_i_1(edgeWeight) neighbour_v_i_2(edgeWeight) ...
  // ...
  explicit Graph(const char* filename)
  {
    std::ifstream iFile(filename);
    std::string line;
    std::getline(iFile, line);
    initialiseGraph(std::stoi(line));
    createEdges(move(iFile));
  }

  Graph()
    : m_numVertices(0)
  {}

  void addVertex(int label)
  {
    if (label < m_vertices.size())
    {
      m_vertices.resize(label+1);
    }
    m_vertices[label] = std::make_shared<Vertex>(label);
    ++m_numVertices;
  }

  Vertex& getVertex(int label)
  {
    return *m_vertices[label];
  }

  Vertex const& getVertex(int label) const
  {
    return *m_vertices[label];
  }

  int getNumVertices() const
  {
    return m_numVertices;
  }

  std::optional<Edge> const& getEdge(Vertex const& v1, Vertex const &v2) const
  {
    return getEdge(v1.getLabel(), v2.getLabel());
  }

  std::optional<Edge> const& getEdge(int v1, int v2) const
  {
    return m_edges[v1][v2];
  }

  std::optional<Edge>& getEdge(int v1, int v2)
  {
    return m_edges[v1][v2];
  }

  std::optional<Edge>& getEdge(Vertex& v1, Vertex& v2)
  {
    return getEdge(v1.getLabel(), v2.getLabel());
  }

  void addEdge(Vertex const& v1, Vertex const& v2, int weight)
  {
    m_edges[v1.getLabel()][v2.getLabel()] = Edge(m_vertices[v1.getLabel()], m_vertices[v2.getLabel()], weight);
  }

  template <typename Func>
  void visitVertices(Func&& func) const
  {
    // Traverse graph and apply function to each vertex
    for (int i = 0; i < m_numVertices; ++i)
    {
      auto&& vertex = getVertex(i);
      for (auto&& neighbour : vertex.getNeighbours())
      {
        if (auto ptrNeighbour = neighbour.lock())
        {
          auto result = func(vertex, *ptrNeighbour);
          if (!result) return;
        }
      }
    }
  }

};
