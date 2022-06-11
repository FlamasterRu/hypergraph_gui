#include "editor.h"
#include "ui_editor.h"

Editor::Editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);
    // PaintArea
}

Editor::~Editor()
{
    delete ui;
}

void Editor::PaintAreaMouseClicked(int posX, int posY, Qt::MouseButton button)
{
    if (button == Qt::LeftButton)   // левой кнопкой начинаем действие
    {
        if (ui->toolButtonAddVertex->isChecked())   // инструмент добавить вершину
        {
            auto vertex = mGraf.addVertex();
            vertex->setPosition(posX, posY);
        }
        if (ui->toolButtonAddEdge->isChecked())     // инструмент добавить ребро
        {
            // левой кнопкой составляем список вершин для ребра
            int vertexId = FindVertex(posX, posY, 10);
            if ( (vertexId != -1) && (!mTempEdge.contains(vertexId)) )
            {
                mTempEdge.push_back(vertexId);
            }
        }
        if (ui->toolButtonDeleteVertex->isChecked())    // инструмент удалить вершину
        {
            int vertexId = FindVertex(posX, posY, 10);
            if (vertexId != -1)
            {
                mGraf.deleteVertex(vertexId);
            }
        }
        if (ui->toolButtonDeleteEdge->isChecked())      // инструмент удалить ребро
        {
            int vertexId = FindVertex(posX, posY, 10);
            if ( (vertexId != -1) && (!mTempEdge.contains(vertexId)) )
            {
                mTempEdge.push_back(vertexId);
            }
        }
    }
    else if (button == Qt::RightButton) // правой кнопкой завершаем действие
    {
        if (ui->toolButtonAddEdge->isChecked()) // инструмент добавить ребро
        {
            // добавляем последнюю вершину к ребру и создаём ребро, если правой кнопкой нажать на пустоту, действие отменяется
            int vertexId = FindVertex(posX, posY, 10);
            if ( (vertexId != -1) && (!mTempEdge.contains(vertexId)) && (mTempEdge.size() >= 1) )
            {
                mTempEdge.push_back(vertexId);
                // чтобы в ребре было минимум две вершины
                auto edge = mGraf.addEdge();
                for (const int id : mTempEdge)
                {
                    auto vertex = mGraf.getVertexByIndex(id);
                    mGraf.linkVertexAndEdge(vertex, edge);
                }
            }
            mTempEdge.clear();
        }
        if (ui->toolButtonDeleteEdge->isChecked())  // инструмент удалить ребро
        {
            int vertexId = FindVertex(posX, posY, 10);
            if ( (vertexId != -1) && (!mTempEdge.contains(vertexId)) )
            {
                mTempEdge.push_back(vertexId);
                QVector<int> deleteEdges = FindEdge(mTempEdge.toSet());
                for (int e : deleteEdges)
                {
                    mGraf.deleteEdge(e);
                }
            }
            mTempEdge.clear();
        }
    }
    SendDrawerStruct( BuildDrawerStruct() );
    //ui->PaintAreaWidget->PaintGraph(&mGraf);
}

bool Editor::WriteGraf(const QString& fileName, const FileType fileType)
{
    return true;
}

bool Editor::ReadGraf(const QString& fileName)
{
    return true;
}

bool Editor::ClearGraf()
{
    return true;
}

void Editor::on_toolButtonCursor_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonAddVertex->setChecked(false);
        ui->toolButtonDeleteVertex->setChecked(false);
        ui->toolButtonAddEdge->setChecked(false);
        ui->toolButtonDeleteEdge->setChecked(false);
    }
}

void Editor::on_toolButtonAddVertex_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonCursor->setChecked(false);
        ui->toolButtonDeleteVertex->setChecked(false);
        ui->toolButtonAddEdge->setChecked(false);
        ui->toolButtonDeleteEdge->setChecked(false);
    }
}

void Editor::on_toolButtonDeleteVertex_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonCursor->setChecked(false);
        ui->toolButtonAddVertex->setChecked(false);
        ui->toolButtonAddEdge->setChecked(false);
        ui->toolButtonDeleteEdge->setChecked(false);
    }
}

void Editor::on_toolButtonAddEdge_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonCursor->setChecked(false);
        ui->toolButtonAddVertex->setChecked(false);
        ui->toolButtonDeleteVertex->setChecked(false);
        ui->toolButtonDeleteEdge->setChecked(false);
    }
}

void Editor::on_toolButtonDeleteEdge_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonCursor->setChecked(false);
        ui->toolButtonAddVertex->setChecked(false);
        ui->toolButtonDeleteVertex->setChecked(false);
        ui->toolButtonAddEdge->setChecked(false);
    }
}

int Editor::FindVertex(const int x, const int y, const double r)
{
    int numNearest = -1;
    double minDist = 1e20;
    auto allVertex = mGraf.getVertexList();
    for (auto vertex : allVertex)
    {
        auto pos = vertex->getPosition();
        double d = std::sqrt( std::pow(x-pos.first, 2.) + std::pow(y-pos.second, 2.) );
        if ( (d < r) && (d < minDist) )
        {
            minDist = d;
            numNearest = vertex->getId();
        }
    }
    return  numNearest;
}

QVector<int> Editor::FindEdge(const QSet<int> vertexex)
{
    QVector<int> result;
    auto allEdges = mGraf.getEdgeList();
    for (auto itE = allEdges.begin(); itE != allEdges.end(); ++itE)
    {
        auto vertexList = (*itE)->getListVertex();
        QSet<int> vertexexId;
        for (auto itV = vertexList.begin(); itV != vertexList.end(); ++itV)
        {
            vertexexId.insert( (*itV)->getId() );
        }
        if (vertexexId.contains(vertexex))
        {
            result.push_back( (*itE)->getId() );
        }
    }
    return result;
}

DrawerStruct Editor::BuildDrawerStruct()
{
    DrawerStruct result;

    QVector<QColor> colorList;
    colorList.push_back(Qt::black);
    colorList.push_back(Qt::red);
    colorList.push_back(Qt::green);
    colorList.push_back(Qt::darkGreen);
    colorList.push_back(Qt::blue);
    colorList.push_back(Qt::cyan);
    colorList.push_back(Qt::darkCyan);
    colorList.push_back(Qt::magenta);
    colorList.push_back(Qt::darkMagenta);
    colorList.push_back(Qt::yellow);
    colorList.push_back(Qt::gray);

    int j = 0;
    QVector<Curve> curves;
    for (auto edge : mGraf.getEdgeList())
    {
        QColor color = colorList.at(j%colorList.size());
        ++j;
        auto it1 = edge->getListVertex().begin();
        auto it2 = it1;
        ++it2;
        for (; it2 != edge->getListVertex().end(); ++it1, ++it2)
        {
            auto pos1 = (*it1)->getPosition();
            auto pos2 = (*it2)->getPosition();
            Curve c(pos1.first, pos1.second, pos2.first, pos2.second);
            c.colors.push_back( color );
            AddCurve(c, curves);
        }
    }
    for (const Curve& c : curves)
    {
        Line l;
        l.x1 = c.x1;
        l.x2 = c.x2;
        l.y1 = c.y1;
        l.y2 = c.y2;
        l.width = 4;
        for (const QColor color : c.colors)
        {
            l.color = color;
            result.lines.push_back(l);
        }
    }

    int i = 0;
    for (auto vertex : mGraf.getVertexList())
    {
        auto pos = vertex->getPosition();
        int x = pos.first;
        int y = pos.second;
        Circle c;
        c.x = x - 20;
        c.y = y - 20;
        c.r = 40;
        c.borderColor = Qt::black;
        c.borderWidth = 3;
        c.fillColor = Qt::white;
        result.circles.push_back(c);

        Text t(x - 20, y - 20, 40, 40, 10, QString::number(vertex->getId()), Qt::black);
        result.texts.push_back(t);
    }
    return result;;
}

void Editor::AddCurve(const Curve& c, QVector<Curve>& curves)
{
    // в curves ищет c, если найдёт, то добавит к кривой цвет
    for (Curve& cur : curves)
    {
        if ( (cur.x1 == c.x1) && (cur.y1 == c.y1) && (cur.x2 == c.x2) && (cur.y2 == c.y2) ) // из A в B
        {
            cur.colors += c.colors;
            return;
        }
        else if ( (cur.x1 == c.x2) && (cur.y1 == c.y2) && (cur.x2 == c.x1) && (cur.y2 == c.y1) )    // из B в А
        {
            cur.colors += c.colors;
            return;
        }
    }
    // если не нашёл, добавляем новую кривую
    curves.push_back(c);
}

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


//bool MainWindow::WriteToFile(const hg::Hypergraphe& graf, const std::string fileName)
//{
//    // гиперграф хранит список номеров всех узлов и их координат и список каждого ребра с ему инцидентными вершинами
//    std::ofstream oFile(fileName);
//    if (!oFile.is_open())
//    {
//        return false;
//    }
//    // записываем вершины
//    auto vertexList = mGraf.getVertexList();
//    oFile << vertexList.size() << "\n";
//    if (vertexList.size() != 0)
//    {
//        int i = 0;
//        for (auto it = vertexList.begin(); it != vertexList.end(); ++it)
//        {
//            oFile << i << " " << (*it)->getPosition().first << " " << (*it)->getPosition().second << "\n";
//            ++i;
//        }
//    }
//    // записываем рёбра
//    auto edgeList = mGraf.getEdgeList();
//    oFile << edgeList.size() << "\n";
//    if (edgeList.size() != 0)
//    {
//        for (auto itE = edgeList.begin(); itE != edgeList.end(); ++itE)
//        {
//            auto vList = (*itE)->getListVertex();
//            oFile << vList.size() << " ";
//            for (auto itV = vList.begin(); itV != vList.end(); ++itV)
//            {
//                oFile << (*itV)->getId() << " ";
//            }
//            oFile << "\n";
//        }
//    }
//    oFile.close();

//    return true;
//}

//bool MainWindow::ReadFromFile(const std::string fileName)
//{
//    std::ifstream iFile(fileName);
//    if (!iFile.is_open())
//    {
//        return false;
//    }
//    mGraf = hg::Hypergraphe();
//    // вершины
//    int vertexCount = 0;
//    iFile >> vertexCount;
//    QVector<int> vertexNums;
//    for (int i = 0; i < vertexCount; ++i)
//    {
//        int tmp, x, y;
//        iFile >> tmp >> x >> y;
//        mGraf.addVertex();
//        mGraf.getVertexByIndex(tmp)->setPosition(x, y);
//    }
//    // рёбра
//    int edgeCount = 0;
//    iFile >> edgeCount;
//    for (int i = 0; i < edgeCount; ++i)
//    {
//        int vCount = 0;
//        iFile >> vCount;
//        int edgeNum = mGraf.addEdge()->getId();
//        for (int j = 0; j < vCount; ++j)
//        {
//            int tmp;
//            iFile >> tmp;
//            mGraf.linkVertexAndEdge(mGraf.getVertexByIndex(tmp), mGraf.getEdgeByIndex(edgeNum));
//        }
//    }

//    return true;
//}






