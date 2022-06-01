#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QTextStream>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QVector>
#include <QVector2D>
#include <cmath>
#include "hypergraph_struct/hypergraph.h"

namespace Ui {
class PaintArea;
}

// стркутура, которая содержит координаты линии, и все цвета этой линии
struct Curve
{
    Curve(){};
    Curve(const int x1_, const int y1_, const int x2_, const int y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}
    int x1;
    int y1;
    int x2;
    int y2;
    QVector<QColor> colors;
};

class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = nullptr);
    ~PaintArea();

    void mousePressEvent(QMouseEvent * event);  // нажатие курсором на виджет

    void paintEvent(QPaintEvent *event);  // вызывает отрисовку на виджет

    void PaintGraph(const hg::Hypergraphe* graph);  // передаёт граф для отрисовки

signals:
    void MouseClicked(int posX, int posY, Qt::MouseButton button);


private:
    // методы
    void FillColorList();   // добавляет цвета в список mColorList
    void AddCurve(const Curve& c, QVector<Curve>& curves);
    void PaintCurves(const QVector<Curve>& curves, QPainter& painter);


    // константы
    const int VERTEXDIAM = 40;  // диаметр вершины
    const int EDGEWIDTH = 4;    // толщина ребра

    // поля
    Ui::PaintArea *ui;

    QVector<QColor> mColorsList;    // список цветов, чтобы все рёбра были цветные
    const hg::Hypergraphe* mGraph = nullptr;
};

#endif // PAINTAREA_H












