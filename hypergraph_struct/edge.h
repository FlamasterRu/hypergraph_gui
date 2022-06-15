#pragma once
#include "type.h"
#include "hypergraph.h"

namespace hg
{

class Edge
{
    friend class Hypergraphe;

protected:

    unsigned int e_Id;
    int e_Weight = 1;
    std::string e_Str = "";
    int e_Size = 4;
    QColor e_Color = Qt::black;

    ListVertex e_ListVertex;

public:

    static const std::shared_ptr<Edge> createEdge(Hypergraphe& gr);

    unsigned int getId() const;
    const hg::ListVertex& getListVertex() const;

    int getWeight() const;
    const std::string& getDateString() const;
    const int getSize() const;
    const QColor getColor() const;

    void setWeight(const int w);
    void setDataString(const std::string& s);
    void setSize(const int size);
    void setColor(const QColor color);

    Edge(const Edge& e) = delete;
    Edge(const Edge&& e) = delete;
    Edge& operator= (const Edge& e) = delete;

private:

    Edge(const unsigned int id);

};

}	// namespace hypgr
