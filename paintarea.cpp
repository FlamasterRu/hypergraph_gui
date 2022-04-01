#include "paintarea.h"
#include "ui_paintarea.h"

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintArea),
    out(stdout)
{
    ui->setupUi(this);
}

PaintArea::~PaintArea()
{
    delete ui;
}

void PaintArea::PaintHypergraph()
{

}

void PaintArea::mousePressEvent(QMouseEvent* event)
{
    out << event->x() << " " << event->y() << endl;
}

































