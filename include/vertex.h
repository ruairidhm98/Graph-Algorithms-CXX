#pragma once

#include <memory>
#include <vector>

class Vertex
{
private:
  std::vector<std::weak_ptr<Vertex> > m_vert;
  const int m_label;
public:

  explicit Vertex(int label);
  int const& getLabel() const;
  std::vector<std::weak_ptr<Vertex> >& getNeighbours();
  void addNeighbour(std::shared_ptr<Vertex>& neighbour);

  // Encapsulate visitng vertices in a function to avoid having to explicitly access the weak pointer outwith the class
  // Function is expected to have some form of return value that would determine if this stopped iterating over neighbours
  // as graph algorithms are essentially search algorithms that can terminate at any given point without exploring the entirety
  template <typename Func>
  void visitNeighbours(Func&& func)
  {
    for (auto&& weakV : m_vert)
    {
      if (auto ptr = weakV.lock())
      {
        auto result = func(*ptr);
        if (!result) return;
      }
    }
  } 

};
