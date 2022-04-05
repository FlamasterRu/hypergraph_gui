#include "paintarea.h"
#include "ui_paintarea.h"

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintArea),
    out(stdout)
{
    ui->setupUi(this);
    FillColorList();
}

PaintArea::~PaintArea()
{
    delete ui;
}

void PaintArea::mousePressEvent(QMouseEvent* event)
{
    int x = event->x();
    int y = event->y();
    out << "PaintArea: " << x << " " << y << " " << event->button() << endl;
    if (event->button() == Qt::LeftButton)
    {
        if (mCurrentState == AddVertex)
        {
            DrawVertex(x, y);
        }
        if (mCurrentState == AddEdge)
        {
            // левой кнопкой добавляем вершины к ребру
            AddVertexToLastEdge(x, y);
        }
    }
    else if (event->button() == Qt::RightButton)
    {
        if (mCurrentState == AddEdge)
        {
            // правой кнопкой завершаем добавление вершин к ребру и создаём это ребро
            DrawEdge(x, y);
        }
    }
}

void PaintArea::DrawVertex(const int x, const int y)
{
    Vertex v;
    v.x = x;
    v.y = y;
    int name = 0;
    for (const Vertex& t : mVertexList)
    {
        if (t.name > name)
            name = t.name;
    }
    v.name = name + 1;
    mVertexList.push_back(v);

    repaint();
}

void PaintArea::DrawEdge(const int x, const int y)
{
    // Если мы добавили последнюю вершину к ребру, то добавляем ребро
    if (AddVertexToLastEdge(x, y))
    {
        // Если у ребра больше 1 вершины
        if (mLastEdge.v.size() > 1)
        {
            mEdgeList.push_back(mLastEdge);
            mLastEdge.v.clear();
            repaint();
        }
    }
    else // отменили добавление ребра
    {
        mLastEdge.v.clear();
    }

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

    int j = 0;
    // Отрисовываем рёбра первыми
    for (const Edge& e : mEdgeList)
    {
        painter.setPen(QPen(mColorsList.at(j%mColorsList.size()), 3));
        ++j;
        for (int i = 0; i < e.v.size() - 1; ++i)
        {
            int x1 = e.v.at(i).x;
            int y1 = e.v.at(i).y;
            int x2 = e.v.at(i+1).x;
            int y2 = e.v.at(i+1).y;
            painter.drawLine(x1, y1, x2, y2);
        }
    }

    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.setFont(QFont("Monospace", 11, 2));
    // отрисовываем вершины в конце, чтобы перекрыть ими рёбра
    int i = 1;
    for (const Vertex& v : mVertexList)
    {
        painter.drawEllipse(v.x - VERTEXDIAM/2, v.y - VERTEXDIAM/2, VERTEXDIAM, VERTEXDIAM);
        // номера вершин после отрисовки вершин
        QRect textRect(v.x - VERTEXDIAM/2, v.y - VERTEXDIAM/2, VERTEXDIAM, VERTEXDIAM);
        painter.drawText(textRect, Qt::AlignCenter, QString::number(i));
        ++i;
    }
}

void PaintArea::ChangeState(const State state)
{
    mCurrentState = state;
}

bool PaintArea::AddVertexToLastEdge(const int x, const int y)
{
    Vertex nearestVertex = FindVertex(x, y, RSEARCH);
    if (nearestVertex.name == -1)
    {
       out << "Can't find vertex in: x=" << x << ", y=" << y << ", r=" << RSEARCH << endl;
       return false;
    }
    else
    {
        // чтобы в одно ребро не добавить несколько раз одну и ту же вершину
        for (const Vertex& v : mLastEdge.v)
        {
            if (v.name == nearestVertex.name)
            {
                out << "Can't add contains vertex: " << v.name << endl;
                return false;
            }
        }
        mLastEdge.v.push_back(nearestVertex);
        return true;
    }
}

PaintArea::Vertex PaintArea::FindVertex(const int x, const int y, const double r)
{
    Vertex nearestVertex;
    nearestVertex.name = -1;
    double minDist = 1e20;
    for (const Vertex& v : mVertexList)
    {
        double d = std::sqrt( std::pow(x-v.x, 2.) + std::pow(y-v.y, 2.) );
        if ( (d < r) && (d < minDist) )
        {
            minDist = d;
            nearestVertex = v;
        }
    }
    return nearestVertex;
}

void PaintArea::FillColorList()
{
    mColorsList.push_back(Qt::black);
    mColorsList.push_back(Qt::red);
    mColorsList.push_back(Qt::darkRed);
    mColorsList.push_back(Qt::green);
    mColorsList.push_back(Qt::darkGreen);
    mColorsList.push_back(Qt::blue);
    mColorsList.push_back(Qt::darkBlue);
    mColorsList.push_back(Qt::cyan);
    mColorsList.push_back(Qt::darkCyan);
    mColorsList.push_back(Qt::magenta);
    mColorsList.push_back(Qt::darkMagenta);
    mColorsList.push_back(Qt::yellow);
    mColorsList.push_back(Qt::darkYellow);
    mColorsList.push_back(Qt::gray);
}


























