#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QTextStream>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QVector>
#include "hypergraph_struct/hypergraph.h"

namespace Ui {
class PaintArea;
}

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


    // константы
    const int VERTEXDIAM = 30;  // диаметр вершины

    // поля
    Ui::PaintArea *ui;
    QTextStream out;    // для отображения в консоль промежуточной информации

    QVector<QColor> mColorsList;    // список цветов, чтобы все рёбра были цветные
    const hg::Hypergraphe* mGraph = nullptr;
};

#endif // PAINTAREA_H












