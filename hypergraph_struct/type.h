#pragma once
#include <list>
#include <map>
#include <armadillo>
#include <memory>
#include <vector>
#include <string>
#include <QColor>


namespace hg
{

	class Hypergraphe;
	class Vertex;
	class Edge;


    typedef arma::SpMat<int>
        AdjacencyMatrix;

	typedef std::list< std::shared_ptr<Vertex> >
		ListVertex;

	typedef std::list< std::shared_ptr<Edge> >
		ListEdge;

	typedef std::map< unsigned int, std::shared_ptr<Vertex> >
		VertexByIndex;

	typedef std::map< unsigned int, std::shared_ptr<Edge> >
		EdgeByIndex;

    enum VertexFigure
    {
        Circle,
        Square
    };

}	// namespace hypgr
