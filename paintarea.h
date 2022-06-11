#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QTextStream>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include "editor.h"

namespace Ui {
class PaintArea;
}


class PaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit PaintArea(QWidget *parent = nullptr);
    ~PaintArea();

    void mousePressEvent(QMouseEvent * event);  // нажатие курсором на виджет

    void paintEvent(QPaintEvent *event);  // вызывает отрисовку на виджет

public slots:
    void Paint(DrawerStruct dStruct);

signals:
    void MouseClicked(int posX, int posY, Qt::MouseButton button);  // посылает сигнал, что нажата кнопка мыши


private:
    // поля
    Ui::PaintArea *ui;

    DrawerStruct mDrawerStruct;
};

#endif // PAINTAREA_H












