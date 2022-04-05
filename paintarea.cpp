#include "paintarea.h"
#include "ui_paintarea.h"

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintArea),
    out(stdout)
{
    ui->setupUi(this);
    mPainter.begin(this);
}

PaintArea::~PaintArea()
{
    delete ui;
}

void PaintArea::mousePressEvent(QMouseEvent* event)
{
    int x = event->x();
    int y = event->y();
    out << "PaintArea: " << x << " " << y << endl;
    switch (mCurrentState)
    {
    case Cursor:
        break;
    case AddVertex:
        DrawVertex(x, y);
        break;
    case DeleteVertex:
        EraseVertex(x, y);
        break;
    case AddEdge:
        DrawEdge(x, y);
        break;
    case DeleteEdge:
        EraseEdge(x, y);
        break;
    default:
        break;
    }
}

void PaintArea::DrawVertex(const int x, const int y)
{
    Vertex v;
    v.x = x;
    v.y = y;
    v.rendered = false;
    mVertexList.push_back(v);
    repaint();
}

void PaintArea::DrawEdge(const int x, const int y)
{
    repaint();
}

void PaintArea::EraseVertex(const int x, const int y)
{
    repaint();
}

void PaintArea::EraseEdge(const int x, const int y)
{
    repaint();
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика

    // отрисовываем вершины в конце, чтобы перекрыть ими рёбра
    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.setFont(QFont("Monospace", 11, 2));
    int i = 1;
    for (Vertex& v : mVertexList)
    {
        painter.drawEllipse(v.x - VERTEXDIAM/2, v.y - VERTEXDIAM/2, VERTEXDIAM, VERTEXDIAM);
        // номера вершин после отрисовки всех вершин
        QRect textRect(v.x - VERTEXDIAM/2, v.y - VERTEXDIAM/2, VERTEXDIAM, VERTEXDIAM);
        painter.drawText(textRect, Qt::AlignCenter, QString::number(i));
        ++i;
    }

}

void PaintArea::ChangeState(const State state)
{
    mCurrentState = state;
}



































