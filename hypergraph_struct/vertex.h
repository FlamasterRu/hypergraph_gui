#pragma once
#include "type.h"
#include "hypergraph.h"


namespace hg
{

class Vertex
{
    friend class Hypergraphe;

protected:

    ///////
    unsigned int v_Id;

    int v_Weight = 1;
    std::string v_Str = "";
    int v_PosX;
    int v_PosY;
    QColor v_Color = Qt::black;
    int v_Size = 40;
    VertexFigure v_Figure = VertexFigure::Circle;

    ListEdge v_ListEdge;
    ///////

public:

    static const std::shared_ptr<Vertex> createVertex(Hypergraphe& gr);

    unsigned int getId() const;
    const hg::ListEdge& getListEdge() const;

    int getWeight() const;
    const std::string& getDateString() const;
    const std::pair<int, int> getPosition() const;
    const QColor getColor() const;
    const int getSize() const;
    const VertexFigure getFigure() const;

    void setWeight(const int w);
    void setDataString(const std::string& s);
    void setPosition(const int posX, const int posY);
    void setColor(const QColor c);
    void setSize(const int size);
    void setFigure(const VertexFigure fig);

    Vertex(const Vertex& v) = delete;
    Vertex(const Vertex&& v) = delete;
    Vertex& operator= (const Vertex& v) = delete;

private:

    Vertex(const unsigned int id);
};

}	// namespace hypgr
