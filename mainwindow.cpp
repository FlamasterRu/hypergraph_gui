#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("Гиперграф"));

    // между PaintArea и Editor
    connect(ui->PaintAreaWidget, SIGNAL(MouseClicked(int, int, Qt::MouseButton)), ui->EditorWidget, SLOT(PaintAreaMouseClicked(int, int, Qt::MouseButton)));
    connect(ui->EditorWidget, SIGNAL(SendDrawerStruct(DrawerStruct)), ui->PaintAreaWidget, SLOT(Paint(DrawerStruct)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCreateNew_triggered()
{
    ui->EditorWidget->ClearGraf();
    mLastFileName = QString();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C://",
                                                    tr("Text files (*.txt)"));
    if (fileName == QString())
    {
        return;
    }
    mLastFileName = fileName;
    ui->EditorWidget->ReadGraf(fileName);
}

void MainWindow::on_actionSave_triggered()
{
    if (mLastFileName == QString())
    {
        on_actionSaveAsCustom_triggered();
    }
    else
    {
        ui->EditorWidget->WriteGraf(mLastFileName, FileType::Custom);
    }
}

void MainWindow::on_actionSaveAsCustom_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "C://",
                                                    tr("Text files (*.txt)"));
    if (fileName == QString())
    {
        return;
    }
    mLastFileName = fileName;
    ui->EditorWidget->WriteGraf(fileName, FileType::Custom);
}

void MainWindow::on_actionSaveAsMatrix_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "C://",
                                                    tr("Text files (*.txt)"));
    if (fileName == QString())
    {
        return;
    }
    mLastFileName = fileName;
    ui->EditorWidget->WriteGraf(fileName, FileType::IncidenceMatrix);
}

void MainWindow::on_actionSaveAsEdges_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "C://",
                                                    tr("Text files (*.txt)"));
    if (fileName == QString())
    {
        return;
    }
    mLastFileName = fileName;
    ui->EditorWidget->WriteGraf(fileName, FileType::EdgeList);
}

void MainWindow::on_actionCloseFile_triggered()
{
    on_actionSave_triggered();
    on_actionCreateNew_triggered();
}

void MainWindow::on_actionOpenPlugins_triggered()
{
}

void MainWindow::on_actionHelp_triggered()
{
}
