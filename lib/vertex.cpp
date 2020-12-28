#include "vertex.h"

Vertex::Vertex(int label)
  : m_label(label)
{}

int const& Vertex::getLabel() const
{
  return m_label;
}

std::vector<std::weak_ptr<Vertex> >& Vertex::getNeighbours()
{
  return m_vert;
}

void Vertex::addNeighbour(std::shared_ptr<Vertex>& neighbour)
{
  m_vert.push_back(neighbour);
}
 