#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
}

void MainWindow::on_actionOpen_triggered()
{
    out << "on_actionOpen_triggered" << endl;
}

void MainWindow::on_actionSave_triggered()
{
    out << "on_actionSave_triggered" << endl;
}

void MainWindow::on_actionSaveAs_triggered()
{
    out << "on_actionSaveAs_triggered" << endl;
}

void MainWindow::on_actionCloseFile_triggered()
{
    out << "on_actionCloseFile_triggered" << endl;
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
    if (checked)
    {
        // выключаем все инструменты
        ui->centralwidget->ChangeState(PaintArea::Cursor);
    }
}

void MainWindow::on_actionAddVertex_triggered(bool checked)
{
    out << "on_actionAddVertex_triggered: " << ui->actionCursor->isChecked() << " " << ui->actionAddVertex->isChecked() << " " << ui->actionDeleteVertex->isChecked() <<
           " " << ui->actionAddEdge->isChecked() << " " << ui->actionDeleteEdge->isChecked() << endl;
    if (checked)
    {
        // добавляем вершину
        ui->centralwidget->ChangeState(PaintArea::AddVertex);
    }
}

void MainWindow::on_actionDeleteVertex_triggered(bool checked)
{
    out << "on_actionDeleteVertex_triggered: " << ui->actionCursor->isChecked() << " " << ui->actionAddVertex->isChecked() << " " << ui->actionDeleteVertex->isChecked() <<
           " " << ui->actionAddEdge->isChecked() << " " << ui->actionDeleteEdge->isChecked() << endl;
    if (checked)
    {
        // удаляем вершину
        ui->centralwidget->ChangeState(PaintArea::DeleteVertex);
    }
}

void MainWindow::on_actionAddEdge_triggered(bool checked)
{
    out << "on_actionAddEdge_triggered: " << ui->actionCursor->isChecked() << " " << ui->actionAddVertex->isChecked() << " " << ui->actionDeleteVertex->isChecked() <<
           " " << ui->actionAddEdge->isChecked() << " " << ui->actionDeleteEdge->isChecked() << endl;
    if (checked)
    {
        // добавляем ребро
        ui->centralwidget->ChangeState(PaintArea::AddEdge);
    }
}

void MainWindow::on_actionDeleteEdge_triggered(bool checked)
{
    out << "on_actionDeleteEdge_triggered: " << ui->actionCursor->isChecked() << " " << ui->actionAddVertex->isChecked() << " " << ui->actionDeleteVertex->isChecked() <<
           " " << ui->actionAddEdge->isChecked() << " " << ui->actionDeleteEdge->isChecked() << endl;
    if (checked)
    {
        // удаляем ребро
        ui->centralwidget->ChangeState(PaintArea::DeleteEdge);
    }
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
            if (vertexId != -1)
            {
                mTempEdge.insert(vertexId);
            }
        }
    }
    else if (button == Qt::RightButton) // правой кнопкой завершаем действие
    {
        if (ui->actionAddEdge->isChecked()) // инструмент добавить ребро
        {
            // добавляем последнюю вершину к ребру и создаём ребро, если правой кнопкой нажать на пустоту, действие отменяется
            int vertexId = FindVertex(posX, posY, RSEARCH);
            if (vertexId != -1)
            {
                mTempEdge.insert(vertexId);
                // чтобы в ребре было минимум две вершины
                if ((mTempEdge.size() >= 2))
                {
                    auto edge = mGraf.addEdge();
                    for (const int id : mTempEdge)
                    {
                        auto vertex = mGraf.getVertexByIndex(id);
                        mGraf.linkVertexAndEdge(vertex, edge);
                    }
                }
            }
            mTempEdge.clear();
        }
    }
    else
    {
        std::cout << mGraf << std::endl;
    }
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























