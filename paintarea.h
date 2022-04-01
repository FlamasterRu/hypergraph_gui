#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QTextStream>
#include <QMouseEvent>

namespace Ui {
class PaintArea;
}

class PaintArea : public QWidget
{
    Q_OBJECT

public:
    explicit PaintArea(QWidget *parent = nullptr);
    ~PaintArea();

    void mousePressEvent(QMouseEvent * event);

    void PaintHypergraph();

private:
    Ui::PaintArea *ui;
    QTextStream out;    // для отображения в консоль промежуточной информации
};

#endif // PAINTAREA_H
