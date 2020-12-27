#pragma once

#include "vertex.h"
#include <memory>

class Edge
{
private:
  std::weak_ptr<Vertex> m_v1;
  std::weak_ptr<Vertex> m_v2;
  int m_weight;
public:

  Edge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, int weight)
    : m_v1(v1)
    , m_v2(v2)
    , m_weight(weight)
  {}

  Edge(Edge&& other)
    : m_v1(other.m_v1)
    , m_v2(other.m_v2)
    , m_weight(other.m_weight)
  {
    other.m_v1.reset();
    other.m_v2.reset();
    other.m_weight = 0;
  }

  int const& getWeight() const
  {
    return m_weight;
  }
};
