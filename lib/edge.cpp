#include "edge.h"
#include "vertex.h"

#include <iostream>

Edge::Edge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, int weight)
  : m_v1(v1)
  , m_v2(v2)
  , m_weight(weight)
  , m_flow(0)
{}

Edge::Edge(Edge&& other) noexcept
  : m_v1(std::move(other.m_v1))
  , m_v2(std::move(other.m_v2))
  , m_weight(other.m_weight)
  , m_flow(other.m_flow)
{
  other.m_weight = 0;
  other.m_flow = 0;
}

Edge& Edge::operator=(Edge&& other) noexcept
{
  m_v1 = std::move(other.m_v1);
  m_v2 = std::move(other.m_v2);
  m_weight = other.m_weight;
  m_flow = other.m_flow;

  other.m_weight = 0;
  other.m_flow = 0;
  return *this;
}

int const& Edge::getWeight() const
{
  return m_weight;
}

void Edge::setFlow(int flow)
{
  m_flow = flow;
}

int const& Edge::getFlow() const
{
  return m_flow;
}

int& Edge::getFlow()
{
  return m_flow;
}

Vertex& Edge::getV1()
{
  return *(m_v1.lock());
}

Vertex& Edge::getV2()
{
  return *(m_v2.lock());
}

Vertex const& Edge::getV1() const
{
  return *(m_v1.lock());
}

Vertex const& Edge::getV2() const
{
  return *(m_v2.lock());
}

void Edge::print(char end) const
{
  std::cout << "{" << getV1().getLabel() << "," << getV2().getLabel() << "}" << end;
}
