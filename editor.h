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
    void SendDrawerStruct(DrawerStruct dStruct);

public slots:
    void PaintAreaMouseClicked(int posX, int posY, Qt::MouseButton button);

private slots:
    void on_toolButtonCursor_clicked(bool checked);
    void on_toolButtonAddVertex_clicked(bool checked);
    void on_toolButtonDeleteVertex_clicked(bool checked);
    void on_toolButtonAddEdge_clicked(bool checked);
    void on_toolButtonDeleteEdge_clicked(bool checked);
    void on_spinBoxVertexSize_valueChanged(int arg1);
    void on_comboBoxVertexFigure_currentIndexChanged(int index);
    void on_comboBoxVertexColor_currentIndexChanged(int index);
    void on_spinBoxEdgeWidth_valueChanged(int arg1);
    void on_comboBoxEdgeColor_currentIndexChanged(int index);

private:

    // для редактирования и отображения
    int FindVertex(const int x, const int y, const double r);
    QVector<int> FindEdge(const QSet<int> vertexex);
    int FindEdge(const int x, const int y);
    DrawerStruct BuildDrawerStruct();
    QColor IndToColor(const int index);
    int ColorToInd(const QColor color);
    void Repaint();
    void InitVertexEditor(const int vertexId);
    void InitEdgeEditor(const int edgeId);
    void MoveLines(QVector<Line>& lines);

    const int FindVertexRadius = 20;

    Ui::Editor *ui;
    hg::Hypergraphe mGraf;
    QList<int> mTempEdge;
    QVector<QColor> mColorList;
    int mChosedVertexId = -1;
    int mChosedEdgeId = -1;
    DrawerStruct mLastStruct;
};


struct Line
{
    Line(){;}
    Line (const int id_, const int x1_, const int y1_, const int x2_, const int y2_, const int width_, const QColor c_)
        : edgeId(id_), x1(x1_), y1(y1_), x2(x2_), y2(y2_), width(width_), color(c_){;}
    int edgeId;
    int x1, y1, x2, y2, width;
    QColor color;
    bool solid = true;
    bool operator== (const Line& r) const
    {
        if ( (x1 == r.x1) && (y1 == r.y1) && (x2 == r.x2) && (y2 == r.y2))
            return true;
        else if ( (x1 == r.x2) && (y1 == r.y2) && (x2 == r.x1) && (y2 == r.y1))
            return true;
        else
            return false;
    }
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
    Square (const int x_, const int y_, const int size_, const int borderWidth_, const QColor borderColor_, const QColor fillColor_)
        : x(x_), y(y_), size(size_), borderWidth(borderWidth_), borderColor(borderColor_), fillColor(fillColor_){;}
    int x, y, size, borderWidth;
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
