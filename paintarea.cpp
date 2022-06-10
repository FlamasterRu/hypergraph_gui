#include "paintarea.h"
#include "ui_paintarea.h"

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintArea)
{
    ui->setupUi(this);
    //FillColorList();
}

PaintArea::~PaintArea()
{
    delete ui;
}

void PaintArea::mousePressEvent(QMouseEvent* event)
{
    emit MouseClicked(event->x(), event->y(), event->button());
}

void PaintArea::Paint(DrawerStruct dStruct)
{
    mDrawerStruct = dStruct;
}

void PaintArea::paintEvent(QPaintEvent *event)
{
//    if (mGraph == nullptr)
//    {
//        return; // чтобы не обращаться к пустому графу при инициализации виджетов
//    }
//    Q_UNUSED(event);
//    QPainter painter(this); // Создаём объект отрисовщика

//    int j = 0;
//    // Отрисовываем рёбра первыми
//    QVector<Curve> curves;
//    for (auto edge : mGraph->getEdgeList())
//    {
//        QColor color = mColorsList.at(j%mColorsList.size());
//        ++j;
//        auto it1 = edge->getListVertex().begin();
//        auto it2 = it1;
//        ++it2;
//        for (; it2 != edge->getListVertex().end(); ++it1, ++it2)
//        {
//            auto pos1 = (*it1)->getPosition();
//            auto pos2 = (*it2)->getPosition();
//            Curve c(pos1.first, pos1.second, pos2.first, pos2.second);
//            c.colors.push_back( color );
//            AddCurve(c, curves);
//        }
//    }
//    PaintCurves(curves, painter);

//    painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::FlatCap));
//    painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
//    painter.setFont(QFont("Monospace", 11, 2));
//    // отрисовываем вершины в конце, чтобы перекрыть ими рёбра
//    int i = 0;
//    for (auto vertex : mGraph->getVertexList())
//    {
//        auto pos = vertex->getPosition();
//        int x = pos.first;
//        int y = pos.second;
//        painter.drawEllipse(x - VERTEXDIAM/2, y - VERTEXDIAM/2, VERTEXDIAM, VERTEXDIAM);
//        // номера вершин после отрисовки вершин
//        QRect textRect(x - VERTEXDIAM/2, y - VERTEXDIAM/2, VERTEXDIAM, VERTEXDIAM);
//        painter.drawText(textRect, Qt::AlignCenter, QString::number(i));
//        ++i;
//    }

    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика

    // сначала всегда рёбра
    for (const Line& l : mDrawerStruct.lines)
    {
        painter.setPen(QPen(l.color, l.width));
        painter.drawLine(l.x1, l.y1, l.x2, l.y2);
    }

    for (const Circle& c : mDrawerStruct.circles)
    {
        painter.setPen(QPen(c.borderColor, c.borderWidth, Qt::SolidLine, Qt::FlatCap));
        painter.setBrush(QBrush(c.fillColor, Qt::SolidPattern));
        painter.drawEllipse(c.x, c.y, c.r, c.r);
    }

    for (const Square& s : mDrawerStruct.squares)
    {
        painter.setPen(QPen(s.borderColor, s.borderWidth, Qt::SolidLine, Qt::FlatCap));
        painter.setBrush(QBrush(s.fillColor, Qt::SolidPattern));
        painter.drawRect(s.x, s.y, s.width, s.height);
    }

    // текст всегда в конце
    for (const Text& t : mDrawerStruct.texts)
    {
        painter.setPen(QPen(t.color, 3, Qt::SolidLine, Qt::FlatCap));
        painter.setFont(QFont("Monospace", t.size, 2));
        QRect textRect(t.x1, t.y1, t.width, t.height);
        painter.drawText(textRect, Qt::AlignCenter, t.text);
    }
}

//void PaintArea::PaintGraph(const hg::Hypergraphe* graph)
//{
//    mGraph = graph;
//    repaint();
//}


//void PaintArea::FillColorList()
//{
//    mColorsList.push_back(Qt::black);
//    mColorsList.push_back(Qt::red);
//    mColorsList.push_back(Qt::green);
//    mColorsList.push_back(Qt::darkGreen);
//    mColorsList.push_back(Qt::blue);
//    mColorsList.push_back(Qt::cyan);
//    mColorsList.push_back(Qt::darkCyan);
//    mColorsList.push_back(Qt::magenta);
//    mColorsList.push_back(Qt::darkMagenta);
//    mColorsList.push_back(Qt::yellow);
//    mColorsList.push_back(Qt::gray);
//}

//void PaintArea::AddCurve(const Curve& c, QVector<Curve>& curves)
//{
//    // в curves ищет c, если найдёт, то добавит к кривой цвет
//    for (Curve& cur : curves)
//    {
//        if ( (cur.x1 == c.x1) && (cur.y1 == c.y1) && (cur.x2 == c.x2) && (cur.y2 == c.y2) ) // из A в B
//        {
//            cur.colors += c.colors;
//            return;
//        }
//        else if ( (cur.x1 == c.x2) && (cur.y1 == c.y2) && (cur.x2 == c.x1) && (cur.y2 == c.y1) )    // из B в А
//        {
//            cur.colors += c.colors;
//            return;
//        }
//    }
//    // если не нашёл, добавляем новую кривую
//    curves.push_back(c);
//}

//void PaintArea::PaintCurves(const QVector<Curve>& curves, QPainter& painter)
//{
//    for (const Curve& c : curves)
//    {
//        if (c.colors.size() == 1)
//        {
//            painter.setPen(QPen(c.colors.first(), EDGEWIDTH));
//            painter.drawLine(c.x1, c.y1, c.x2, c.y2);
//        }
//        else
//        {
//            // строим несколько параллельных прямых
//            double x1 = c.x1, x2 = c.x2, y1 = c.y1, y2 = c.y2;
//            // находим перпендикуляр к прямой между двумя точками
//            QVector2D n( (y1-y2)/(x2 - x1), 1 );
//            n.normalize();  // нормируем

//            // начальные координаты набора параллельных прямых
//            double x0l = x1 - ((EDGEWIDTH/2)*(c.colors.size()-1) + 1) * n.x();
//            double y0l = y1 - ((EDGEWIDTH/2)*(c.colors.size()-1) + 1) * n.y();
//            double x0r = x2 - ((EDGEWIDTH/2)*(c.colors.size()-1) + 1) * n.x();
//            double y0r = y2 - ((EDGEWIDTH/2)*(c.colors.size()-1) + 1) * n.y();
//            for (int i = 0; i < c.colors.size(); ++i)
//            {
//                double nx1 = x0l + n.x()*i*(EDGEWIDTH+1);
//                double ny1 = y0l + n.y()*i*(EDGEWIDTH+1);
//                double nx2 = x0r + n.x()*i*(EDGEWIDTH+1);
//                double ny2 = y0r + n.y()*i*(EDGEWIDTH+1);

//                painter.setPen(QPen(c.colors.at(i), EDGEWIDTH));
//                painter.drawLine(nx1, ny1, nx2, ny2);
//            }
//        }
//    }
//}






















