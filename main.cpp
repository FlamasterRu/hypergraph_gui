#include "mainwindow.h"
#include "hypergraph_struct/hypergraph.h"
#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    hg::Hypergraphe g;
    auto v1 = g.addVertex();
    auto v2 = g.addVertex();
    auto v3 = g.addVertex();
    auto v4 = g.addVertex();
    auto v5 = g.addVertex();

    auto e1 = g.addEdge();
    auto e2 = g.addEdge();
    auto e3 = g.addEdge();
    auto e4 = g.addEdge();

    g.linkVertexAndEdge(v1, e1);
    g.linkVertexAndEdge(v2, e2);
    g.linkVertexAndEdge(v3, e3);
    g.linkVertexAndEdge(v4, e4);
    g.linkVertexAndEdge(v5, e1);

    std::cout << g << std::endl;

    return 0;

    QApplication a(argc, argv);
    //a.setApplicationName("Гиперграф");
    MainWindow w;
    w.show();
    return a.exec();
}
