#include "edge.h"
#include "graph.h"
#include "vertex.h"

#include <fstream>
#include <sstream>
#include <string>

// Assume file is in format
// line 0| first line is an integer containing the number of vertices
// line i| neighbour_v_i_1(edgeWeight) neighbour_v_i_2(edgeWeight) ...
// ...
Graph::Graph(const char* filename)
{
  std::ifstream iFile(filename);
  std::string line;
  std::getline(iFile, line);
  initialiseGraph(std::stoi(line));
  createEdges(std::move(iFile));
}

Graph::Graph()
  : m_numVertices(0)
{}

// Reads the file
void Graph::createEdges(std::ifstream&& iFile)
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

void Graph::initialiseGraph(int numVertices)
{
  m_numVertices = numVertices;
  m_edges.resize(numVertices);
  for (int i = 0; i < m_numVertices; ++i)
  {
    m_vertices.push_back(std::make_shared<Vertex>(i));
    m_edges[i].resize(m_numVertices);
  }
}

inline void Graph::addEdge(std::shared_ptr<Vertex>& v1, std::shared_ptr<Vertex>& v2, int weight)
{
  m_edges[v1->getLabel()][v2->getLabel()] = std::move(Edge(v1, v2, weight));
}

void Graph::addVertex(int label)
{
  if (label < m_vertices.size())
  {
    m_vertices.resize(label+1);
  }
  m_vertices[label] = std::make_shared<Vertex>(label);
  ++m_numVertices;
}

std::shared_ptr<Vertex> Graph::getVertex(int label)
{
  return m_vertices[label];
}

Vertex const& Graph::getVertex(int label) const
{
  return *m_vertices[label];
}

int Graph::getNumVertices() const
{
  return m_numVertices;
}

std::optional<Edge> const& Graph::getEdge(Vertex const& v1, Vertex const &v2) const
{
  return getEdge(v1.getLabel(), v2.getLabel());
}

std::optional<Edge> const& Graph::getEdge(int v1, int v2) const
{
  return m_edges[v1][v2];
}

std::optional<Edge>& Graph::getEdge(int v1, int v2)
{
  return m_edges[v1][v2];
}

std::optional<Edge>& Graph::getEdge(Vertex& v1, Vertex& v2)
{
  return getEdge(v1.getLabel(), v2.getLabel());
}

void Graph::addEdge(int v1, int v2, int weight)
{
  m_edges[v1][v2] = Edge(m_vertices[v1], m_vertices[v2], weight);
  m_vertices[v1]->addNeighbour(m_vertices[v2]);
}

void Graph::addEdge(Vertex const& v1, Vertex const& v2, int weight)
{
  addEdge(v1.getLabel(), v2.getLabel(), weight);
}
