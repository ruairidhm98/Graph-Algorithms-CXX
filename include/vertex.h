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

  std::vector<std::weak_ptr<Vertex> >& getNeighbours()
  {
    return m_vert;
  }

  void addNeighbour(std::shared_ptr<Vertex>& neighbour)
  {
    m_vert.push_back(neighbour);
  }

  template <typename Func>
  // Encapsulate visitng vertices in a function to avoid having to explicitly access the weak pointer outwith the class
  void visitNeighbours(Func&& func)
  {
    for (auto&& weakV : m_vert)
    {
      if (auto ptr = weakV.lock())
      {
        auto stop = func(*ptr);
        if (stop) return;
      }
    }
  } 

};
