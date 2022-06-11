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

struct Curve;
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
    void SendDrawerStruct(DrawerStruct dStruct);

public slots:
    void PaintAreaMouseClicked(int posX, int posY, Qt::MouseButton button);


private slots:
    void on_toolButtonCursor_clicked(bool checked);

    void on_toolButtonAddVertex_clicked(bool checked);

    void on_toolButtonDeleteVertex_clicked(bool checked);

    void on_toolButtonAddEdge_clicked(bool checked);

    void on_toolButtonDeleteEdge_clicked(bool checked);

private:

    // для редактирования и отображения
    int FindVertex(const int x, const int y, const double r);
    QVector<int> FindEdge(const QSet<int> vertexex);
    DrawerStruct BuildDrawerStruct();
    void AddCurve(const Curve& c, QVector<Curve>& curves);


    Ui::Editor *ui;
    hg::Hypergraphe mGraf;
    QList<int> mTempEdge;
};

struct Curve
{
    Curve(){};
    Curve(const int x1_, const int y1_, const int x2_, const int y2_) : x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}
    int x1;
    int y1;
    int x2;
    int y2;
    QVector<QColor> colors;
};

struct Line
{
    Line(){;}
    Line (const int x1_, const int y1_, const int x2_, const int y2_, const int width_, const QColor c_)
        : x1(x1_), y1(y1_), x2(x2_), y2(y2_), width(width_), color(c_){;}
    int x1, y1, x2, y2, width;
    QColor color;
};

struct Circle
{
    Circle(){;}
    Circle (const int x_, const int y_, const int r_, const int borderWidth_, const QColor borderColor_, const QColor fillColor_)
        : x(x_), y(y_), r(r_), borderWidth(borderWidth_), borderColor(borderColor_), fillColor(fillColor_){;}
    int x, y, r, borderWidth;
    QColor borderColor, fillColor;
};

struct Square
{
    Square(){;}
    Square (const int x_, const int y_, const int width_, const int height_, const int borderWidth_, const QColor borderColor_, const QColor fillColor_)
        : x(x_), y(y_), width(width_), height(height_), borderWidth(borderWidth_), borderColor(borderColor_), fillColor(fillColor_){;}
    int x, y, width, height, borderWidth;
    QColor borderColor, fillColor;
};

struct Text
{
    Text(){;}
    Text (const int x_, const int y_, const int width_, const int height_, const int size_, const QString& text_, const QColor c_)
        : x(x_), y(y_), width(width_), height(height_), size(size_), text(text_), color(c_){;}
    int x, y, width, height, size;
    QString text;
    QColor color;
};


#endif // DRAWER_H
