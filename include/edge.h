#pragma once

#include <memory>

class Vertex;

class Edge
{
private:
  std::weak_ptr<Vertex> m_v1;
  std::weak_ptr<Vertex> m_v2;
  int m_weight;
  int m_flow;

public:
  Edge(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, int weight);
  Edge(Edge&& other) noexcept;
  Edge& operator=(Edge&& other) noexcept;
  int const& getWeight() const;
  void setFlow(int flow);
  int const& getFlow() const;
  int& getFlow();
  Vertex& getV1();
  Vertex& getV2();
  Vertex const& getV1() const;
  Vertex const& getV2() const;
  void print(char end='\n') const;
};
