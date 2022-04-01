#include "edge.h"

namespace hg
{

	Edge::Edge(const unsigned int id) :
		e_Id(id)
	{
	}

	const std::shared_ptr<Edge> Edge::createEdge(Hypergraphe& gr)
	{
		return gr.addEdge();
	}

	const hg::ListVertex& Edge::getListVertex() const
	{
		return e_ListVertex;
	}

	unsigned int Edge::getId() const
	{
		return e_Id;
	}

	int Edge::getWeight() const
	{
		return e_Weight;
	}

	const std::string& Edge::getDateString() const
	{
		return e_Str;
	}

	void Edge::setWeight(const int w)
	{
		e_Weight = w;
	}

	void Edge::setDataString(const std::string& s)
	{
		e_Str = s;
	}

}	// namespace hypgr