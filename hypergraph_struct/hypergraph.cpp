#include "hypergraph.h"
#include <iostream>
#include <fstream>

namespace hg
{

	//////////////////// constructor and destructor
	Hypergraphe::Hypergraphe()
	{
        h_AdjacencyMatrixInt.resize(0, 0);
	}

	Hypergraphe::Hypergraphe(const AdjacencyMatrix& matrix)
	{
        for (int i = 0; i < matrix.n_rows; ++i)
		{
			this->addVertex();
		}
        for (int i = 0; i < matrix.n_cols; ++i)
		{
			this->addEdge();
		}
        for (int i = 0; i < matrix.n_rows; ++i)
		{
            for (int j = 0; j < matrix.n_cols; ++j)
			{
                if (matrix(i, j) == 1)
				{
					this->linkVertexAndEdge((*this)(i), (*this)[j]);
				}
			}
		}
	}

	Hypergraphe::Hypergraphe(const std::string fileName)
	{
		std::ifstream file(fileName);
		if (!file.is_open())
		{
			throw("Can't open input file.\n");
		}
		int n_row, n_col;
		file >> n_row >> n_col;
		if (n_row <= 0)
		{
			throw("Incorrect row size in input file.\n");
		}
		if (n_col <= 0)
		{
			throw("Incorrect col size in input file.\n");
		}
		for (int i = 0; i < n_row; ++i)
		{
			this->addVertex();
		}
		for (int i = 0; i < n_col; ++i)
		{
			this->addEdge();
		}
		for (int i = 0; i < n_row; ++i)
		{
			for (int j = 0; j < n_col; ++j)
			{
				int temp;
				file >> temp;
				if (temp == 1)
				{
					this->linkVertexAndEdge((*this)(i), (*this)[j]);
				}
			}
		}
	}

	Hypergraphe::Hypergraphe(const Hypergraphe& h)
	{
		for (int i = 0; i < h.getNumVertex(); ++i)
		{
			this->addVertex();
			(*this)(i)->setDataString(h(i)->getDateString());
			(*this)(i)->setWeight(h(i)->getWeight());
		}
		for (int i = 0; i < h.getNumEdge(); ++i)
		{
			this->addEdge();
			(*this)[i]->setDataString(h[i]->getDateString());
			(*this)[i]->setWeight(h[i]->getWeight());
		}
		for (int i = 0; i < h.getNumVertex(); ++i)
		{
			for (int j = 0; j < h.getNumEdge(); ++j)
			{
				if (h.isVertexInEdge( h(i), h[j]))
				{
					this->linkVertexAndEdge((*this)(i), (*this)[j]);
				}
			}
		}
	}

	Hypergraphe& Hypergraphe::operator= (const Hypergraphe& h)
	{
        h_AdjacencyMatrixInt.resize(h.getNumVertex(), h.getNumEdge());
		this->h_Index_Edge = h.h_Index_Edge;
		this->h_Index_Vertex = h.h_Index_Vertex;
		this->h_IsAdjacencyMatrixActual = false;
		this->h_ListEdge = h.h_ListEdge;
		this->h_ListVertex = h.h_ListVertex;
		this->h_NumOfEdge = h.h_NumOfEdge;
		this->h_NumOfVertex = h.h_NumOfVertex;
		return *this;
	}

	Hypergraphe::~Hypergraphe()
	{

	}
	///////////////////////////////////////////////////////


	////////////////////	add 
	const std::shared_ptr<Vertex> Hypergraphe::addVertex()
	{
		h_IsAdjacencyMatrixActual = false;

		std::shared_ptr<Vertex> temp(new Vertex(h_NumOfVertex));

		h_ListVertex.push_back(temp);
		h_Index_Vertex.insert(std::pair< unsigned int, std::shared_ptr<Vertex> >(h_NumOfVertex, temp));

		++h_NumOfVertex;
		return temp;
	}

	const std::shared_ptr<Edge> Hypergraphe::addEdge()
	{
		h_IsAdjacencyMatrixActual = false;

		std::shared_ptr<Edge> temp(new Edge(h_NumOfEdge));

		h_ListEdge.push_back(temp);
		h_Index_Edge.insert(std::pair< unsigned int, std::shared_ptr<Edge> >(h_NumOfEdge, temp));

		++h_NumOfEdge;
		return temp;
	}
	///////////////////////////////////////////////////////


	////////////////////	get
	const std::shared_ptr<Vertex> Hypergraphe::getVertexByIndex(const unsigned int index) const
	{
		if (index >= h_NumOfVertex)
		{
			throw("index >= h_NumOfVertex");
		}
		return (*h_Index_Vertex.find(index)).second;
	}

	const std::shared_ptr<Edge> Hypergraphe::getEdgeByIndex(const unsigned int index) const
	{
		if (index >= h_NumOfEdge)
		{
			throw("index >= h_NumOfEdge");
		}
		return (*h_Index_Edge.find(index)).second;
	}

	const std::shared_ptr<Vertex> Hypergraphe::operator ()(const unsigned int index) const
	{
		if (index >= h_NumOfVertex)
		{
			throw("index >= h_NumOfVertex");
		}
		return (*h_Index_Vertex.find(index)).second;
	}

	const std::shared_ptr<Edge> Hypergraphe::operator [](const unsigned int index) const
	{
		if (index >= h_NumOfEdge)
		{
			throw("index >= h_NumOfEdge");
		}
		return (*h_Index_Edge.find(index)).second;
	}

	const ListVertex& Hypergraphe::getVertexList() const
	{
		return const_cast<const ListVertex&>(h_ListVertex);
	}

	const ListEdge& Hypergraphe::getEdgeList() const
	{
		return const_cast<const ListEdge&>(h_ListEdge);
	}

	unsigned int Hypergraphe::getNumEdge() const
	{
		return h_ListEdge.size();
	}

	unsigned int Hypergraphe::getNumVertex() const
	{
		return h_ListVertex.size();
	}

	unsigned int Hypergraphe::getVertexId(const std::shared_ptr<Vertex>& v) const
	{
		return v->getId();
	}

	unsigned int Hypergraphe::getEdgeId(const std::shared_ptr<Edge>& e) const
	{
		return e->getId();
	}

	const std::string& Hypergraphe::getVertexDateString(const unsigned int index)
	{
		if (index >= h_NumOfVertex)
		{
			throw("index >= h_NumOfVertex");
		}
		return (*h_Index_Vertex.find(index)).second->getDateString();
	}

	const std::string& Hypergraphe::getVertexDateString(const std::shared_ptr<Vertex>& v)
	{
		return v->getDateString();
	}

	int Hypergraphe::getVertexWeight(const unsigned int index)
	{
		if (index >= h_NumOfVertex)
		{
			throw("index >= h_NumOfVertex");
		}
		return (*h_Index_Vertex.find(index)).second->getWeight();
	}

	int Hypergraphe::getVertexWeight(const std::shared_ptr<Vertex>& v)
	{
		return v->getWeight();
	}

	const std::string& Hypergraphe::getEgdeDateString(const unsigned int index)
	{
		if (index >= h_NumOfEdge)
		{
			throw("index >= h_NumOfEdge");
		}
		return (*h_Index_Edge.find(index)).second->getDateString();
	}

	const std::string& Hypergraphe::getEgdeDateString(const std::shared_ptr<Edge>& e)
	{
		return e->getDateString();
	}

	int Hypergraphe::getEdgeWeight(const unsigned int index)
	{
		if (index >= h_NumOfEdge)
		{
			throw("index >= h_NumOfEdge");
		}
		return (*h_Index_Edge.find(index)).second->getWeight();
	}

	int Hypergraphe::getEdgeWeight(const std::shared_ptr<Edge>& e)
	{
		return e->getWeight();
	}

	const AdjacencyMatrix& Hypergraphe::getAdjacencyMatrix()
	{
		if (h_IsAdjacencyMatrixActual)
		{
			return h_AdjacencyMatrixInt;
		}
		else
		{
            h_AdjacencyMatrixInt.resize(h_NumOfVertex, h_NumOfEdge);
			for (auto it1 = h_ListVertex.begin(); it1 != h_ListVertex.end(); ++it1)
			{
				for (auto it2 = (*it1).get()->getListEdge().begin(); it2 != (*it1).get()->getListEdge().end(); ++it2)
				{
                    h_AdjacencyMatrixInt( (*it1).get()->v_Id, (*it2).get()->e_Id ) = 1;
				}
			}
			h_IsAdjacencyMatrixActual = true;
		}
	}
	///////////////////////////////////////////////////////


	////////////////////	link vertex and edge
	bool Hypergraphe::linkVertexAndEdge(const std::shared_ptr<Vertex>& v, const std::shared_ptr<Edge>& e)
	{
		h_IsAdjacencyMatrixActual = false;

		v.get()->v_ListEdge.push_back(e);
		e.get()->e_ListVertex.push_back(v);

		return true;
	}

	bool Hypergraphe::linkVertexToListEdge(const std::shared_ptr<Vertex>& v, const hg::ListEdge& e)
	{
		h_IsAdjacencyMatrixActual = false;

		for (auto it = e.begin(); it != e.end(); ++it)
		{
			v.get()->v_ListEdge.push_back(*it);
			(*it).get()->e_ListVertex.push_back(v);
		}

		return true;
	}

	bool Hypergraphe::linkEdgeToListVertex(const std::shared_ptr<Edge>& e, const hg::ListVertex& v)
	{
		h_IsAdjacencyMatrixActual = false;

		for (auto it = v.begin(); it != v.end(); ++it)
		{
			e.get()->e_ListVertex.push_back(*it);
			(*it).get()->v_ListEdge.push_back(e);
		}

		return true;
	}

	const std::shared_ptr<Edge>  Hypergraphe::linkListVertex(const hg::ListVertex& v)
	{
		h_IsAdjacencyMatrixActual = false;

		auto temp = this->addEdge();
		this->linkEdgeToListVertex(temp, v);

		return temp;
	}

	const std::shared_ptr<Vertex>  Hypergraphe::linkListEdge(const hg::ListEdge& e)
	{
		h_IsAdjacencyMatrixActual = false;

		auto temp = this->addVertex();
		this->linkVertexToListEdge(temp, e);

		return temp;
	}
	///////////////////////////////////////////////////////


	////////////////////
	std::ostream& operator<< (std::ostream& out, Hypergraphe& h)
	{
		h.getAdjacencyMatrix();
		for (int i = 0; i < h.h_NumOfVertex; ++i)
		{
			for (int j = 0; j < h.h_NumOfEdge; ++j)
			{
				out.width(4);
				if (j == h.h_NumOfEdge - 1)
				{
                    out << h.h_AdjacencyMatrixInt(i, j);
				}
				else
				{
                    out << h.h_AdjacencyMatrixInt(i, j) << " ";
				}
			}
			out << "\n";
		}
		return out;
	}

	bool Hypergraphe::isVertexInEdge(const std::shared_ptr<Vertex> v, const std::shared_ptr<Edge> e) const
	{
		bool temp(false);
		if (v.get()->getListEdge().size() < e.get()->getListVertex().size())
		{
			for (auto it = v.get()->getListEdge().begin(); it != v.get()->getListEdge().end(); ++it)
			{
				if ((*it) == e)
				{
					temp = true;
				}
			}
		}
		else
		{
			for (auto it = e.get()->getListVertex().begin(); it != e.get()->getListVertex().end(); ++it)
			{
				if ((*it) == v)
				{
					temp = true;
				}
			}
		}
		return temp;
	}
	///////////////////////////////////////////////////////

}	// namespace hypgr
