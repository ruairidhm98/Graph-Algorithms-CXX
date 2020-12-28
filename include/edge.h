#pragma once

#include "vertex.h"
#include <memory>

class Edge
{
private:
  std::weak_ptr<Vertex> m_v1;
  std::weak_ptr<Vertex> m_v2;
  int m_weight;
  int m_flow;
public:

  Edge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, int weight)
    : m_v1(v1)
    , m_v2(v2)
    , m_weight(weight)
    , m_flow(0)
  {}

  Edge(Edge&& other) noexcept
    : m_v1(std::move(other.m_v1))
    , m_v2(std::move(other.m_v2))
    , m_weight(other.m_weight)
    , m_flow(other.m_flow)
  {
    other.m_weight = 0;
    other.m_flow = 0;
  }

  Edge& operator=(Edge&& other) noexcept
  {
    m_v1 = std::move(other.m_v1);
    m_v2 = std::move(other.m_v2);
    m_weight = other.m_weight;
    m_flow = other.m_flow;

    other.m_weight = 0;
    other.m_flow = 0;
    return *this;
  }

  int const& getWeight() const
  {
    return m_weight;
  }

  void setFlow(int flow)
  {
    m_flow = flow;
  }

  int const& getFlow() const
  {
    return m_flow;
  }

  int& getFlow()
  {
    return m_flow;
  }

  Vertex& getV1()
  {
    return *(m_v1.lock());
  }

  Vertex& getV2()
  {
    return *(m_v2.lock());
  }

  Vertex const& getV1() const
  {
    return *(m_v1.lock());
  }

  Vertex const& getV2() const
  {
    return *(m_v2.lock());
  }

  void print(char end='\n') const
  {
    std::cout << "{" << getV1().getLabel() << "," << getV2().getLabel() << "}" << end;
  }

};
