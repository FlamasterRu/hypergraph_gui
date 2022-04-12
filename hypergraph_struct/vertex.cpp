#include "vertex.h"

namespace hg
{

	Vertex::Vertex(const unsigned int id) :
		v_Id(id)
	{
	}

	const std::shared_ptr<Vertex> Vertex::createVertex(Hypergraphe& gr)
	{
		return gr.addVertex();
	}

	const hg::ListEdge& Vertex::getListEdge() const
	{
		return v_ListEdge;
	}

	unsigned int Vertex::getId() const
	{
		return v_Id;
	}

	int Vertex::getWeight() const
	{
		return v_Weight;
	}

	const std::string& Vertex::getDateString() const
	{
		return v_Str;
	}

    const std::pair<int, int> Vertex::getPosition() const
    {
        return std::pair<int, int>(v_PosX, v_PosY);
    }

	void Vertex::setWeight(const int w)
	{
		v_Weight = w;
	}

	void Vertex::setDataString(const std::string& s)
	{
		v_Str = s;
	}

    void Vertex::setPosition(const int posX, const int posY)
    {
        v_PosX = posX;
        v_PosY = posY;
    }

}	// namespace hypgr
