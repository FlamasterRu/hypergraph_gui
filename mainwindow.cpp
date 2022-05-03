#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <fstream>
#include <QDir>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    out(stdout)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("Гиперграф"));

    // чтобы можно было выбрать только один инструмент для редактирования
    mTools = new QActionGroup(this);
    mTools->addAction(ui->actionCursor);
    mTools->addAction(ui->actionAddVertex);
    mTools->addAction(ui->actionDeleteVertex);
    mTools->addAction(ui->actionAddEdge);
    mTools->addAction(ui->actionDeleteEdge);
    ui->actionCursor->setChecked(true);

    // пересылаем нажатие мыши для обработки основным виджетом
    connect(ui->centralwidget, SIGNAL(MouseClicked(int, int, Qt::MouseButton)), this, SLOT(PaintAreaMouseClicked(int, int, Qt::MouseButton)));
}

MainWindow::~MainWindow()
{
    delete mTools;
    delete ui;
}

void MainWindow::on_actionCreateNew_triggered()
{
    out << "on_actionCreateNew_triggered" << endl;
    mGraf = hg::Hypergraphe();
    ui->centralwidget->PaintGraph(&mGraf);
    mLastFileName = QString();
}

void MainWindow::on_actionOpen_triggered()
{
    out << "on_actionOpen_triggered" << endl;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C://",
                                                    tr("Text files (*.txt)"));
    out << fileName << endl;
    if (fileName == QString())
    {
        return;
    }
    ReadFromFile(fileName.toStdString());
    mLastFileName = fileName;
    ui->centralwidget->PaintGraph(&mGraf);
}

void MainWindow::on_actionSave_triggered()
{
    out << "on_actionSave_triggered " << mLastFileName << endl;
    if (mLastFileName == QString())
    {
        on_actionSaveAs_triggered();
    }
    else
    {
        WriteToFile(mGraf, mLastFileName.toStdString());
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    out << "on_actionSaveAs_triggered" << endl;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "C://",
                                                    tr("Text files (*.txt)"));
    out << fileName << endl;
    if (fileName == QString())
    {
        return;
    }
    mLastFileName = fileName;
    WriteToFile(mGraf, mLastFileName.toStdString());
}

void MainWindow::on_actionCloseFile_triggered()
{
    out << "on_actionCloseFile_triggered" << endl;
    on_actionSave_triggered();
    on_actionCreateNew_triggered();
}

void MainWindow::on_actionOpenPlugins_triggered()
{
    out << "on_actionOpenPlugins_triggered" << endl;
}

void MainWindow::on_actionHelp_triggered()
{
    out << "on_actionHelp_triggered" << endl;
}

void MainWindow::on_actionCursor_triggered(bool checked)
{
    out << "on_actionCursor_triggered: " << ui->actionCursor->isChecked() << " " << ui->actionAddVertex->isChecked() << " " << ui->actionDeleteVertex->isChecked() <<
        " " << ui->actionAddEdge->isChecked() << " " << ui->actionDeleteEdge->isChecked() << endl;
}

void MainWindow::PaintAreaMouseClicked(int posX, int posY, Qt::MouseButton button)
{
    out << "MainWindow: " << posX << " " << posY << " " << button << endl;
    if (button == Qt::LeftButton)   // левой кнопкой начинаем действие
    {
        if (ui->actionAddVertex->isChecked())   // инструмент добавить вершину
        {
            auto vertex = mGraf.addVertex();
            vertex->setPosition(posX, posY);
        }
        if (ui->actionAddEdge->isChecked())     // инструмент добавить ребро
        {
            // левой кнопкой составляем список вершин для ребра
            int vertexId = FindVertex(posX, posY, RSEARCH);
            if ( (vertexId != -1) && (!mTempEdge.contains(vertexId)) )
            {
                mTempEdge.push_back(vertexId);
            }
        }
        if (ui->actionDeleteVertex->isChecked())    // инструмент удалить вершину
        {
            int vertexId = FindVertex(posX, posY, RSEARCH);
            if (vertexId != -1)
            {
                mGraf.deleteVertex(vertexId);
            }
        }
        if (ui->actionDeleteEdge->isChecked())      // инструмент удалить ребро
        {
            int vertexId = FindVertex(posX, posY, RSEARCH);
            if ( (vertexId != -1) && (!mTempEdge.contains(vertexId)) )
            {
                mTempEdge.push_back(vertexId);
            }
        }
    }
    else if (button == Qt::RightButton) // правой кнопкой завершаем действие
    {
        if (ui->actionAddEdge->isChecked()) // инструмент добавить ребро
        {
            // добавляем последнюю вершину к ребру и создаём ребро, если правой кнопкой нажать на пустоту, действие отменяется
            int vertexId = FindVertex(posX, posY, RSEARCH);
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
        if (ui->actionDeleteEdge->isChecked())  // инструмент удалить ребро
        {
            int vertexId = FindVertex(posX, posY, RSEARCH);
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
    else
    {
        std::cout << mGraf << std::endl;
    }
    ui->centralwidget->PaintGraph(&mGraf);
}

int MainWindow::FindVertex(const int x, const int y, const double r)
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

QVector<int> MainWindow::FindEdge(const QSet<int> vertexex)
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

bool MainWindow::WriteToFile(const hg::Hypergraphe& graf, const std::string fileName)
{
    // гиперграф хранит список номеров всех узлов и их координат и список каждого ребра с ему инцидентными вершинами
    std::ofstream oFile(fileName);
    if (!oFile.is_open())
    {
        return false;
    }
    // записываем вершины
    auto vertexList = mGraf.getVertexList();
    oFile << vertexList.size() << "\n";
    if (vertexList.size() != 0)
    {
        int i = 0;
        for (auto it = vertexList.begin(); it != vertexList.end(); ++it)
        {
            oFile << i << " " << (*it)->getPosition().first << " " << (*it)->getPosition().second << "\n";
            ++i;
        }
    }
    // записываем рёбра
    auto edgeList = mGraf.getEdgeList();
    oFile << edgeList.size() << "\n";
    if (edgeList.size() != 0)
    {
        for (auto itE = edgeList.begin(); itE != edgeList.end(); ++itE)
        {
            auto vList = (*itE)->getListVertex();
            oFile << vList.size() << " ";
            for (auto itV = vList.begin(); itV != vList.end(); ++itV)
            {
                oFile << (*itV)->getId() << " ";
            }
            oFile << "\n";
        }
    }
    oFile.close();

    return true;
}

bool MainWindow::ReadFromFile(const std::string fileName)
{
    std::ifstream iFile(fileName);
    if (!iFile.is_open())
    {
        return false;
    }
    mGraf = hg::Hypergraphe();
    // вершины
    int vertexCount = 0;
    iFile >> vertexCount;
    QVector<int> vertexNums;
    for (int i = 0; i < vertexCount; ++i)
    {
        int tmp, x, y;
        iFile >> tmp >> x >> y;
        mGraf.addVertex();
        mGraf.getVertexByIndex(tmp)->setPosition(x, y);
    }
    // рёбра
    int edgeCount = 0;
    iFile >> edgeCount;
    for (int i = 0; i < edgeCount; ++i)
    {
        int vCount = 0;
        iFile >> vCount;
        int edgeNum = mGraf.addEdge()->getId();
        for (int j = 0; j < vCount; ++j)
        {
            int tmp;
            iFile >> tmp;
            mGraf.linkVertexAndEdge(mGraf.getVertexByIndex(tmp), mGraf.getEdgeByIndex(edgeNum));
        }
    }

    return true;
}



















