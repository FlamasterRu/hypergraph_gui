#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>

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

    void on_actionCloseFile_triggered();

    void on_actionOpenPlugins_triggered();

    void on_actionHelp_triggered();

    void on_actionSaveAsCustom_triggered();

    void on_actionSaveAsMatrix_triggered();

    void on_actionSaveAsEdges_triggered();

private:
    // поля
    Ui::MainWindow *ui;

    QString mLastFileName;
};
#endif // MAINWINDOW_H
