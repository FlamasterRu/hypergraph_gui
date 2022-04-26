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
    emit MouseClicked(event->x(), event->y(), event->button());
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    out << "PaintArea::paintEvent: " << mGraph << endl;
    if (mGraph == nullptr)
    {
        return;
    }
    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика

    int j = 0;
    // Отрисовываем рёбра первыми
    for (auto edge : mGraph->getEdgeList())
    {
        painter.setPen(QPen(mColorsList.at(j%mColorsList.size()), 3));
        ++j;
        auto it1 = edge->getListVertex().begin();
        auto it2 = it1;
        ++it2;
        for (; it2 != edge->getListVertex().end(); ++it1, ++it2)
        {
            auto pos1 = (*it1)->getPosition();
            auto pos2 = (*it2)->getPosition();
            painter.drawLine(pos1.first, pos1.second, pos2.first, pos2.second);
        }
    }

    painter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.setFont(QFont("Monospace", 11, 2));
    // отрисовываем вершины в конце, чтобы перекрыть ими рёбра
    int i = 0;
    for (auto vertex : mGraph->getVertexList())
    {
        auto pos = vertex->getPosition();
        int x = pos.first;
        int y = pos.second;
        painter.drawEllipse(x - VERTEXDIAM/2, y - VERTEXDIAM/2, VERTEXDIAM, VERTEXDIAM);
        // номера вершин после отрисовки вершин
        QRect textRect(x - VERTEXDIAM/2, y - VERTEXDIAM/2, VERTEXDIAM, VERTEXDIAM);
        painter.drawText(textRect, Qt::AlignCenter, QString::number(i));
        ++i;
    }
}

void PaintArea::PaintGraph(const hg::Hypergraphe* graph)
{
    mGraph = graph;
    repaint();
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


























