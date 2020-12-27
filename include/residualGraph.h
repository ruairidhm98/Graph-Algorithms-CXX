#pragma once

#include "network.h"
#include <list>

class ResidualGraph
{
private:
  Graph m_graph;

public:
  ResidualGraph(Network& net)
  {
    // Initialise the vertices and edges of the graph
    m_graph.initialiseGraph(net.getGraph().getNumVertices());
    
    Graph& g = net.getGraph();
    g.visitVertices([this,&g](Vertex const& v1, Vertex const& v2)
    {
      Edge const& e = g.getEdge(v1, v2).value();
      // If we can still push flow through the edge and if the edge has not already been created
      if (e.getFlow() < e.getWeight() && !m_graph.getEdge(v1, v2).has_value())
      {
        m_graph.addEdge(v1, v2, e.getWeight() - e.getFlow());
      }
      // If we can 'reverse' the flow of the edge and if the edge has not already been created
      if (e.getFlow() > 0 && !m_graph.getEdge(v2, v1).has_value())
      {
        m_graph.addEdge(v2, v1, std::move(e.getFlow()));
      }
      return true;
    });
  }

  std::list<Edge> findAugmentingPath()
  {
    std::list<Edge> vertices;
    // This *should* be a bitset implementation
    std::vector<bool> visited(m_graph.getNumVertices());

    // We need to carry out a breadth first search, keeping track of predecessors
  }


};

/*
public LinkedList<Edge> findAugmentingPath() {

        LinkedList<Edge> path = new LinkedList<Edge>();

        // Carry out a breadth first search, keeping track of the predecessors
        int pred[] = new int[numVertices];
        Queue<Vertex> queue = new LinkedList<Vertex>();

        // Initialisation
        int i = 0;
        for (Vertex v : vertices) {
            v.visited = false;
            pred[i++] = -1;
        }

        // The path starts from the source, so make this the first vertex to be processed, and clearly
        // it has no predecessor in terms of this path, so use -1 as a flag to indicate no predecessor
        queue.add(source);
        pred[source.getLabel()] = -1;
        source.visited = true;
        boolean exit = false;
        while(queue.size() > 0 && !exit) {
            Vertex u = queue.poll();
            int uLabel = u.getLabel();
            for (Vertex v : adjLists.get(uLabel)) {
                int vLabel = v.getLabel();
            	if (!v.visited) {
                    v.visited = true;
                    pred[vLabel] = uLabel;
                    if (vLabel == sink.getLabel()) {
                        exit = true;
                        break;
                    }
	                queue.add(v);
            	}
            }
        }

        // Trace the path from the sink to the source, adding each edge
        int cursor = sink.getLabel();
        // While there is still edges on the path
        while (cursor != -1 && pred[cursor] != -1) {
            path.addFirst(adjMatrix[pred[cursor]][cursor]);
            cursor = pred[cursor];
        }

        return path;
    }


*/
