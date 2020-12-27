#pragma once

#include <memory>
#include <iostream>
#include <vector>

class Vertex
{
private:
  std::vector<std::weak_ptr<Vertex> > m_vert;
  const int m_label;
public:
  explicit Vertex(int label)
    : m_label(label)
  {}

  int const& getLabel() const
  {
    return m_label;
  }

  std::vector<std::weak_ptr<Vertex> > const& getNeighbours() const
  {
    return m_vert;
  }

  std::vector<std::weak_ptr<Vertex> >& getNeighburs()
  {
    return m_vert;
  }

  void addNeighbour(std::shared_ptr<Vertex>& neighbour)
  {
    m_vert.push_back(neighbour);
  }

};
