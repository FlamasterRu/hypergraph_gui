#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QTextStream>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QVector>

namespace Ui {
class PaintArea;
}

class PaintArea : public QWidget
{
    Q_OBJECT
private:
    struct Vertex
    {
        int x;
        int y;
        int name;   // уникальное имя вершины
    };
    struct Edge
    {
        QVector<Vertex> v;  // список вершин, инцидентных ребру
    };

public:
    explicit PaintArea(QWidget *parent = nullptr);
    ~PaintArea();

    enum State {Cursor, AddVertex, DeleteVertex, AddEdge, DeleteEdge};

    void mousePressEvent(QMouseEvent * event);  // нажатие курсором на виджет

    void paintEvent(QPaintEvent *event);  // вызывает отрисовку на виджет

    void ChangeState(const State state);     // меняет инструмент для отрисовки

signals:
    void MouseClicked(int posX, int posY, Qt::MouseButton button);




private:
    // методы
    void DrawVertex(const int x, const int y);
    void DrawEdge(const int x, const int y);
    void EraseVertex(const int x, const int y);
    void EraseEdge(const int x, const int y);

    Vertex FindVertex(const int x, const int y, const double r);  // ищет вершину, близжайшую к координатам и внутри радиуса (если не найдёт, вернёт вершину с именем -1)
    bool AddVertexToLastEdge(const int x, const int y); // добавляет вершину к создаваемому ребру

    void FillColorList();   // добавляет цвета в список mColorList





    // константы
    const int VERTEXDIAM = 30;  // диаметр вершины
    const double RSEARCH = 20.; // радиус поиска вершины при нажатии на виджет


    // поля
    Ui::PaintArea *ui;
    QTextStream out;    // для отображения в консоль промежуточной информации

    State mCurrentState = Cursor;    // выбранный инструмент для редактирования

    QVector<Vertex> mVertexList;    // список всех вершин

    Edge mLastEdge; // список вершин для добавляемого ребра
    QVector<Edge> mEdgeList;    // список всех рёбер
    QVector<QColor> mColorsList;    // список цветов, чтобы все рёбра были цветные
};

#endif // PAINTAREA_H












