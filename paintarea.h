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

public:
    explicit PaintArea(QWidget *parent = nullptr);
    ~PaintArea();

    enum State {Cursor, AddVertex, DeleteVertex, AddEdge, DeleteEdge};

    void mousePressEvent(QMouseEvent * event);  // нажатие курсором на виджет

    void paintEvent(QPaintEvent *event);  // вызывает отрисовку на виджет

    void ChangeState(const State state);     // меняет инструмент для отрисовки




private:
    // методы
    void DrawVertex(const int x, const int y);
    void DrawEdge(const int x, const int y);
    void EraseVertex(const int x, const int y);
    void EraseEdge(const int x, const int y);



    // константы
    const int VERTEXDIAM = 30;


    // поля
    Ui::PaintArea *ui;
    QTextStream out;    // для отображения в консоль промежуточной информации
    QPainter mPainter;  // в нём рисуются фигуры

    State mCurrentState = Cursor;    // выбранный инструмент для редактирования

    struct Vertex
    {
        int x;
        int y;
        bool rendered = false;
    };

    QVector<Vertex> mVertexList;

};

#endif // PAINTAREA_H
