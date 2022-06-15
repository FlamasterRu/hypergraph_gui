#include "editor.h"
#include "ui_editor.h"
#include <random>
#include <ctime>
#include <QList>
#include <QVector2D>
#include <fstream>

Editor::Editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    ui->editorStackedWidget->setVisible(false);
    mColorList.push_back(Qt::black);
    mColorList.push_back(Qt::red);
    mColorList.push_back(Qt::green);
    mColorList.push_back(Qt::darkGreen);
    mColorList.push_back(Qt::blue);
    mColorList.push_back(Qt::cyan);
    mColorList.push_back(Qt::magenta);
    mColorList.push_back(Qt::darkMagenta);
    mColorList.push_back(Qt::yellow);
    mColorList.push_back(Qt::gray);

    QStringList colors;
    colors << QString::fromUtf8("Чёрный") << QString::fromUtf8("Красный") << QString::fromUtf8("Зелёный") <<
             QString::fromUtf8("Тёмно-зелёный") << QString::fromUtf8("Синий") << QString::fromUtf8("Голубой") <<
             QString::fromUtf8("Розовый") << QString::fromUtf8("Фиолетовый") << QString::fromUtf8("Жёлтый") << QString::fromUtf8("Серый");
    ui->comboBoxVertexColor->addItems(colors);
    ui->comboBoxVertexFigure->addItems( QStringList() << QString::fromUtf8("Круг") << QString::fromUtf8("Квадрат") );
    ui->comboBoxEdgeColor->addItems(colors);
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
            int vertexId = FindVertex(posX, posY, FindVertexRadius);
            if ( (vertexId != -1) && (!mTempEdge.contains(vertexId)) )
            {
                mTempEdge.push_back(vertexId);
            }
        }
        if (ui->toolButtonDeleteVertex->isChecked())    // инструмент удалить вершину
        {
            int vertexId = FindVertex(posX, posY, FindVertexRadius);
            if (vertexId != -1)
            {
                mGraf.deleteVertex(vertexId);
            }
        }
        if (ui->toolButtonDeleteEdge->isChecked())      // инструмент удалить ребро
        {
            int vertexId = FindVertex(posX, posY, FindVertexRadius);
            if ( (vertexId != -1) && (!mTempEdge.contains(vertexId)) )
            {
                mTempEdge.push_back(vertexId);
            }
        }
        if (ui->toolButtonCursor->isChecked())  // режим редактирования
        {
            int vertexId = FindVertex(posX, posY, FindVertexRadius);
            if (vertexId != -1)
            {
                mChosedVertexId = vertexId;
                ui->editorStackedWidget->setVisible(true);
                ui->editorStackedWidget->setCurrentIndex(0);
                InitVertexEditor(vertexId);
            }
            else
            {
                ui->editorStackedWidget->setVisible(false);
                mChosedVertexId = -1;

                int edgeId = FindEdge(posX, posY);
                if (edgeId != -1)
                {
                    mChosedEdgeId = edgeId;
                    ui->editorStackedWidget->setVisible(true);
                    ui->editorStackedWidget->setCurrentIndex(1);
                    InitEdgeEditor(edgeId);
                }
                else
                {
                    ui->editorStackedWidget->setVisible(false);
                    mChosedEdgeId = -1;
                }
            }
        }
    }
    else if (button == Qt::RightButton) // правой кнопкой завершаем действие
    {
        std::mt19937 r;
        r.seed(std::time(nullptr));
        if (ui->toolButtonAddEdge->isChecked()) // инструмент добавить ребро
        {
            // добавляем последнюю вершину к ребру и создаём ребро, если правой кнопкой нажать на пустоту, действие отменяется
            int vertexId = FindVertex(posX, posY, FindVertexRadius);
            if ( (vertexId != -1) && (!mTempEdge.contains(vertexId)) && (mTempEdge.size() >= 1) )
            {
                mTempEdge.push_back(vertexId);
                // чтобы в ребре было минимум две вершины
                auto edge = mGraf.addEdge();
                edge->setColor( mColorList[r()%mColorList.size()] );
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
            int vertexId = FindVertex(posX, posY, FindVertexRadius);
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
        if (ui->toolButtonCursor->isChecked())  // режим редактирования вершин
        {
            if (mChosedVertexId != -1)
            {
                auto vertex = mGraf.getVertexByIndex(mChosedVertexId);
                vertex->setPosition(posX, posY);
            }
        }
    }
    Repaint();
}
void Editor::Repaint()
{
    mLastStruct = BuildDrawerStruct();

    SendDrawerStruct( mLastStruct );
}

bool Editor::WriteGraf(const QString& fileName, const FileType fileType)
{
    std::ofstream oFile(fileName.toStdString());
    oFile << fileType << "\n";
    if (!oFile.is_open())
    {
        return false;
    }
    if (fileType == FileType::Custom)
    {
        mGraf.WriteToFileCustom(oFile);
    }
    else if (fileType == FileType::EdgeList)
    {
        mGraf.WriteToFileEdgeList(oFile);
    }
    else if (fileType == FileType::IncidenceMatrix)
    {
        mGraf.WriteToFileMatrix(oFile);
    }
    oFile.close();
    return true;
}
bool Editor::ReadGraf(const QString& fileName)
{
    std::ifstream iFile(fileName.toStdString());
    if (!iFile.is_open())
    {
        return false;
    }
    int type;
    iFile >> type;
    if (type == FileType::Custom)
    {
        mGraf.ReadFromFileCustom(iFile);
    }
    else if (type == FileType::EdgeList)
    {
        mGraf.ReadFromFileEdgeList(iFile);
    }
    else if (type == FileType::IncidenceMatrix)
    {
        mGraf.ReadFromFileMatrix(iFile);
    }
    iFile.close();
    return true;
}
bool Editor::ClearGraf()
{
    mGraf = hg::Hypergraphe();
    return true;
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
int product(int Px, int Py, int Ax, int Ay, int Bx, int By)
{
  return (Bx - Ax) * (Py - Ay) - (By - Ay) * (Px - Ax);
}
int Editor::FindEdge(const int x, const int y)
{
    int result = -1;
    for (const Line& l : mLastStruct.lines)
    {
        QVector2D n( (static_cast<double>(l.y1)-static_cast<double>(l.y2))/
                     (static_cast<double>(l.x2) - static_cast<double>(l.x1)), 1 );
        n.normalize();

        double x1 = l.x1 + n.x()*l.width/2.;
        double y1 = l.y1 + n.y()*l.width/2.;
        double x2 = l.x1 - n.x()*l.width/2.;
        double y2 = l.y1 - n.y()*l.width/2.;

        double x3 = l.x2 - n.x()*l.width/2.;
        double y3 = l.y2 - n.y()*l.width/2.;
        double x4 = l.x2 + n.x()*l.width/2.;
        double y4 = l.y2 + n.y()*l.width/2.;

        int p1 = product(x, y, x1, y1, x2, y2);
        int p2 = product(x, y, x2, y2, x3, y3);
        int p3 = product(x, y, x3, y3, x4, y4);
        int p4 = product(x, y, x4, y4, x1, y1);
        if ((p1 < 0 && p2 < 0 && p3 < 0 && p4 < 0) ||
            (p1 > 0 && p2 > 0 && p3 > 0 && p4 > 0))
        {
            result = l.edgeId;
        }
    }
    return result;
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

    for (auto edge : mGraf.getEdgeList())
    {
        QColor color = edge->getColor();
        int size = edge->getSize();
        auto it1 = edge->getListVertex().begin();
        auto it2 = it1;
        ++it2;
        for (; it2 != edge->getListVertex().end(); ++it1, ++it2)
        {
            auto pos1 = (*it1)->getPosition();
            auto pos2 = (*it2)->getPosition();
            Line l(edge->getId(), pos1.first, pos1.second, pos2.first, pos2.second, size, color);
            if (edge->getId() == mChosedEdgeId)
            {
                l.solid = false;
            }
            result.lines.push_back(l);
        }
    }
    MoveLines(result.lines);

    for (auto vertex : mGraf.getVertexList())
    {
        QColor fillColor = Qt::white;
        if (vertex->getId() == mChosedVertexId)
        {
            fillColor = Qt::yellow;
        }
        auto pos = vertex->getPosition();
        int x = pos.first;
        int y = pos.second;
        int size = vertex->getSize();
        QColor color = vertex->getColor();
        hg::VertexFigure f = vertex->getFigure();
        if (f == hg::VertexFigure::Circle)
        {
            Circle c(x - size/2, y - size/2, size, 3, color, fillColor);
            result.circles.push_back(c);
        }
        else if (f == hg::VertexFigure::Square)
        {
            Square s(x - size/2, y - size/2, size, 3, color, fillColor);
            result.squares.push_back(s);
        }
        Text t(x - size/2, y - size/2, size, size, size / 4, QString::number(vertex->getId()), Qt::black);
        result.texts.push_back(t);
    }
    return result;;
}

void Editor::MoveLines(QVector<Line>& lines)
{
    QList<int> movedInd;
    for (int i = 0; i < lines.size()-1; ++i)
    {
        QVector<int> toMoveInd;
        if (movedInd.contains(i))
        {
            continue;
        }
        movedInd.push_back(i);
        toMoveInd.push_back(i);
        for (int j = i+1; j < lines.size(); ++j)
        {
            if ( lines[i] == lines[j] )
            {
                if (!movedInd.contains(j))
                {
                    toMoveInd.push_back(j);
                    movedInd.push_back(j);
                }
            }
        }
        if (toMoveInd.size() >= 2)
        {
            Line l0 = lines[toMoveInd.first()];
            double x1 = l0.x1, x2 = l0.x2, y1 = l0.y1, y2 = l0.y2;
            QVector2D n( (y1-y2)/(x2 - x1), 1 );
            n.normalize();
            double allWidth = toMoveInd.size() - 1;
            for (int index : toMoveInd)
            {
                allWidth += lines[index].width;
            }
            double x0l = x1 - (allWidth/2. - lines[toMoveInd.first()].width/2) * n.x();
            double y0l = y1 - (allWidth/2. - lines[toMoveInd.first()].width/2) * n.y();
            double x0r = x2 - (allWidth/2. - lines[toMoveInd.first()].width/2) * n.x();
            double y0r = y2 - (allWidth/2. - lines[toMoveInd.first()].width/2) * n.y();
            for (int index : toMoveInd)
            {
                Line l = lines[index];
                Line nl(l.edgeId, x0l, y0l, x0r, y0r, l.width, l.color);
                nl.solid = l.solid;
                lines[index] = nl;
                x0l += (l.width + 2.) * n.x();
                y0l += (l.width + 2.) * n.y();
                x0r += (l.width + 2.) * n.x();
                y0r += (l.width + 2.) * n.y();
            }
        }
    }
}

void Editor::on_toolButtonCursor_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonAddVertex->setChecked(false);
        ui->toolButtonDeleteVertex->setChecked(false);
        ui->toolButtonAddEdge->setChecked(false);
        ui->toolButtonDeleteEdge->setChecked(false);

        ui->editorStackedWidget->setVisible(false);
    }
    else
    {
        ui->editorStackedWidget->setVisible(false);
        mChosedVertexId = -1;
    }
    Repaint();
}
void Editor::on_toolButtonAddVertex_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonCursor->setChecked(false);
        ui->toolButtonDeleteVertex->setChecked(false);
        ui->toolButtonAddEdge->setChecked(false);
        ui->toolButtonDeleteEdge->setChecked(false);

        ui->editorStackedWidget->setVisible(false);
        mChosedVertexId = -1;
    }
    Repaint();
}
void Editor::on_toolButtonDeleteVertex_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonCursor->setChecked(false);
        ui->toolButtonAddVertex->setChecked(false);
        ui->toolButtonAddEdge->setChecked(false);
        ui->toolButtonDeleteEdge->setChecked(false);

        ui->editorStackedWidget->setVisible(false);
        mChosedVertexId = -1;
    }
    Repaint();
}
void Editor::on_toolButtonAddEdge_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonCursor->setChecked(false);
        ui->toolButtonAddVertex->setChecked(false);
        ui->toolButtonDeleteVertex->setChecked(false);
        ui->toolButtonDeleteEdge->setChecked(false);

        ui->editorStackedWidget->setVisible(false);
        mChosedVertexId = -1;
    }
    Repaint();
}
void Editor::on_toolButtonDeleteEdge_clicked(bool checked)
{
    if (checked)
    {
        ui->toolButtonCursor->setChecked(false);
        ui->toolButtonAddVertex->setChecked(false);
        ui->toolButtonDeleteVertex->setChecked(false);
        ui->toolButtonAddEdge->setChecked(false);

        ui->editorStackedWidget->setVisible(false);
        mChosedVertexId = -1;
    }
    Repaint();
}

QColor Editor::IndToColor(const int index)
{
    if (index == 0)
    {
        return Qt::black;
    }
    else if (index == 1)
    {
        return Qt::red;
    }
    else if (index == 2)
    {
        return Qt::green;
    }
    else if (index == 3)
    {
        return Qt::darkGreen;
    }
    else if (index == 4)
    {
        return Qt::blue;
    }
    else if (index == 5)
    {
        return Qt::cyan;
    }
    else if (index == 6)
    {
        return Qt::magenta;
    }
    else if (index == 7)
    {
        return Qt::darkMagenta;
    }
    else if (index == 8)
    {
        return Qt::yellow;
    }
    else if (index == 9)
    {
        return Qt::gray;
    }
    else
    {
        return Qt::black;
    }
}
int Editor::ColorToInd(const QColor color)
{
    if (color == Qt::black)
    {
        return 0;
    }
    else if (color == Qt::red)
    {
        return 1;
    }
    else if (color == Qt::green)
    {
        return 2;
    }
    else if (color == Qt::darkGreen)
    {
        return 3;
    }
    else if (color == Qt::blue)
    {
        return 4;
    }
    else if (color == Qt::cyan)
    {
        return 5;
    }
    else if (color == Qt::magenta)
    {
        return 6;
    }
    else if (color == Qt::darkMagenta)
    {
        return 7;
    }
    else if (color == Qt::yellow)
    {
        return 8;
    }
    else if (color == Qt::gray)
    {
        return 9;
    }
    else
    {
        return 0;
    }
}

void Editor::InitEdgeEditor(const int edgeId)
{
    auto edge = mGraf.getEdgeByIndex(edgeId);
    ui->spinBoxEdgeWidth->setValue(edge->getSize());
    ui->comboBoxEdgeColor->setCurrentIndex(ColorToInd(edge->getColor()));
}
void Editor::on_spinBoxEdgeWidth_valueChanged(int arg1)
{
    if (mChosedEdgeId != -1)
    {
        auto edge = mGraf.getEdgeByIndex(mChosedEdgeId);
        edge->setSize(arg1);
        Repaint();
    }
}
void Editor::on_comboBoxEdgeColor_currentIndexChanged(int index)
{
    if (mChosedEdgeId != -1)
    {
        auto edge = mGraf.getEdgeByIndex(mChosedEdgeId);
        edge->setColor(IndToColor(index));
        Repaint();
    }
}

void Editor::InitVertexEditor(const int vertexId)
{
    auto vertex = mGraf.getVertexByIndex(vertexId);
    ui->spinBoxVertexSize->setValue(vertex->getSize());
    ui->comboBoxVertexColor->setCurrentIndex(ColorToInd(vertex->getColor()));
    if (vertex->getFigure() == hg::VertexFigure::Circle)
    {
        ui->comboBoxVertexFigure->setCurrentIndex(0);
    }
    else
    {
        ui->comboBoxVertexFigure->setCurrentIndex(1);
    }

}
void Editor::on_spinBoxVertexSize_valueChanged(int arg1)
{
    if (mChosedVertexId != -1)
    {
        auto vertex = mGraf.getVertexByIndex(mChosedVertexId);
        vertex->setSize(arg1);
        Repaint();
    }
}
void Editor::on_comboBoxVertexFigure_currentIndexChanged(int index)
{
    if (mChosedVertexId != -1)
    {
        auto vertex = mGraf.getVertexByIndex(mChosedVertexId);
        if (index == 0)
        {
            vertex->setFigure(hg::VertexFigure::Circle);
        }
        else if (index == 1)
        {
            vertex->setFigure(hg::VertexFigure::Square);
        }
        Repaint();
    }
}
void Editor::on_comboBoxVertexColor_currentIndexChanged(int index)
{
    if (mChosedVertexId != -1)
    {
        auto vertex = mGraf.getVertexByIndex(mChosedVertexId);
        vertex->setColor(IndToColor(index));
        Repaint();
    }
}





