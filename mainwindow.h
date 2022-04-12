#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QTextStream>
#include "hypergraph_struct/hypergraph.h"
#include <QSet>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void PaintAreaMouseClicked(int posX, int posY, Qt::MouseButton button);

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
    // методы
    int FindVertex(const int x, const int y, const double r);    // ищет вершину в окружности радиусом r

    // константы
    const double RSEARCH = 20.; // радиус поиска вершины при нажатии на виджет

    // поля
    Ui::MainWindow *ui;
    QTextStream out;    // для отображения в консоль промежуточной информации


    QActionGroup* mTools;

    hg::Hypergraphe mGraf;
    QSet<int> mTempEdge;
};
#endif // MAINWINDOW_H
