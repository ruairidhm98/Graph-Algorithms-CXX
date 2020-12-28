#pragma once

#include <optional>
#include <memory>
#include <vector>

class Edge;
class Vertex;

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

  void createEdges(std::ifstream&& iFile);
  void initialiseGraph(int numVertices);
  void addEdge(std::shared_ptr<Vertex>& v1, std::shared_ptr<Vertex>& v2, int weight);

public:

  explicit Graph(const char* filename);
  Graph();
  void addVertex(int label);
  std::shared_ptr<Vertex> getVertex(int label);
  Vertex const& getVertex(int label) const;
  int getNumVertices() const;
  std::optional<Edge> const& getEdge(Vertex const& v1, Vertex const &v2) const;
  std::optional<Edge> const& getEdge(int v1, int v2) const;
  std::optional<Edge>& getEdge(int v1, int v2);
  std::optional<Edge>& getEdge(Vertex& v1, Vertex& v2);
  void addEdge(int v1, int v2, int weight);
  void addEdge(Vertex const& v1, Vertex const& v2, int weight);

  // Visits every edge in the graph, func is expected to return an indicator that signifies whether search can stop.
  template <typename Func>
  void Graph::visitEdges(Func&& func)
  {
    // Traverse graph and apply function to each vertex
    for (int i = 0; i < m_numVertices; ++i)
    {
      auto&& vertex = getVertex(i);
      for (auto&& neighbour : vertex->getNeighbours())
      {
        if (auto ptrNeighbour = neighbour.lock())
        {
          auto result = func(getEdge(*vertex, *ptrNeighbour).value());
          if (!result) return;
        }
      }
    }
  }

};
