#ifndef DRAWER_H
#define DRAWER_H

#include <QWidget>
#include <QVector>
#include "hypergraph_struct/hypergraph.h"

namespace Ui {
class Editor;
}

enum FileType
{
    Custom,
    EdgeList,
    IncidenceMatrix
};

struct Line;
struct Circle;
struct Square;
struct Text;

struct DrawerStruct
{
    QVector<Line> lines;
    QVector<Circle> circles;
    QVector<Square> squares;
    QVector<Text> texts;
};

class Editor : public QWidget
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr);
    ~Editor();

    bool WriteGraf(const QString& fileName, const FileType fileType);
    bool ReadGraf(const QString& fileName);

    bool ClearGraf();

signals:
    // для области отображения
    void SendDrawerStruct(DrawerStruct dStruct);


public slots:
    // от области отображения
    void PaintAreaMouseClicked(int posX, int posY, Qt::MouseButton button);


private:
    Ui::Editor *ui;
};


struct Line
{
    int x1, y1, x2, y2, width;
    QColor color;
};

struct Circle
{
    int x, y, r, borderWidth;
    QColor borderColor, fillColor;
};

struct Square
{
    int x, y, width, height, borderWidth;
    QColor borderColor, fillColor;
};

struct Text
{
    int x1, y1, width, height, size;
    QString text;
    QColor color;
};


#endif // DRAWER_H
