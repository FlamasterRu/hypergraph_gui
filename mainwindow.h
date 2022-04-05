#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QTextStream>
#include "hypergraph_struct/hypergraph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionCreateNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionCloseFile_triggered();

    void on_actionOpenPlugins_triggered();

    void on_actionHelp_triggered();

    void on_actionCursor_triggered(bool checked);

    void on_actionAddVertex_triggered(bool checked);

    void on_actionDeleteVertex_triggered(bool checked);

    void on_actionAddEdge_triggered(bool checked);

    void on_actionDeleteEdge_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    QTextStream out;    // для отображения в консоль промежуточной информации


    QActionGroup* mTools;

    hg::Hypergraphe mGraf;
};
#endif // MAINWINDOW_H
