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
}

MainWindow::~MainWindow()
{
    delete mTools;
    delete ui;
}

void MainWindow::on_actionCreateNew_triggered()
{
    out << "on_actionCreateNew_triggered" << endl;
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



























