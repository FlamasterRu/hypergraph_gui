#include "paintarea.h"
#include "ui_paintarea.h"

PaintArea::PaintArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintArea)
{
    ui->setupUi(this);
}

PaintArea::~PaintArea()
{
    delete ui;
}

void PaintArea::mousePressEvent(QMouseEvent* event)
{
    emit MouseClicked(event->x(), event->y(), event->button());
}

void PaintArea::Paint(DrawerStruct dStruct)
{
    mDrawerStruct = dStruct;
    repaint();
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика

    // сначала всегда рёбра
    for (const Line& l : mDrawerStruct.lines)
    {
        painter.setPen(QPen(l.color, l.width));
        if (!l.solid)
        {
            painter.setPen(QPen(l.color, l.width, Qt::DashLine));
        }
        painter.drawLine(l.x1, l.y1, l.x2, l.y2);
    }

    for (const Circle& c : mDrawerStruct.circles)
    {
        painter.setPen(QPen(c.borderColor, c.borderWidth, Qt::SolidLine, Qt::FlatCap));
        painter.setBrush(QBrush(c.fillColor, Qt::SolidPattern));
        painter.drawEllipse(c.x, c.y, c.r, c.r);
    }

    for (const Square& s : mDrawerStruct.squares)
    {
        painter.setPen(QPen(s.borderColor, s.borderWidth, Qt::SolidLine, Qt::FlatCap));
        painter.setBrush(QBrush(s.fillColor, Qt::SolidPattern));
        painter.drawRect(s.x, s.y, s.size, s.size);
    }

    // текст всегда в конце
    for (const Text& t : mDrawerStruct.texts)
    {
        painter.setPen(QPen(t.color, 3, Qt::SolidLine, Qt::FlatCap));
        painter.setFont(QFont("Monospace", t.size, 2));
        QRect textRect(t.x, t.y, t.width, t.height);
        painter.drawText(textRect, Qt::AlignCenter, t.text);
    }
}
